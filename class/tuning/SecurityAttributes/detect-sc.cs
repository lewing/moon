// Copyright (C) 2009 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

using System;
using System.Collections.Generic;
using System.IO;

using Mono.Cecil;
using Mono.Cecil.Cil;

using Moonlight.SecurityModel;

/// <summary>
/// Detect code that needs to be [SecurityCritical] in order to be executable 
/// under the CoreCLR security model. The ouput includes comments as to why 
/// the attribute is needed.
/// 
/// Note: Results are not always 100% certain so a comment is added wrt the
/// condition that was used to report it as a candidate.
/// </summary>
class Program {

	static SortedDictionary<string, string> methods = new SortedDictionary<string, string> ();

	// note: pointers don't *need* to be SecurityCritical because they can't be
	// used without a "unsafe" or "fixed" context that transparent code won't support
	static bool CheckType (TypeReference type)
	{
		string fullname = type.FullName;

		// pointers can only be used by fixed/unsafe code
		return !fullname.EndsWith ("*");
	}

	static string CheckVerifiability (MethodDefinition method)
	{
		if (!method.HasBody)
			return String.Empty;

		foreach (Instruction ins in method.Body.Instructions) {
			switch (ins.OpCode.Code) {
			case Code.No:		// ecma 335, part III, 2.2
			case Code.Calli:	// Lidin p260
			case Code.Cpblk:	// ecma 335, part III, 3.30
			case Code.Initblk:	// ecma 335, part III, 3.36
			case Code.Jmp:		// ecma 335, part III, 3.37 / Lidin p259
			case Code.Localloc:	// ecma 335, part III, 3.47
				return ins.OpCode.Name;
			case Code.Arglist:	// lack test case
			case Code.Cpobj:
				return ins.OpCode.Name;
			case Code.Mkrefany:	// not 100% certain
				return ins.OpCode.Name;
			}
		}
		return String.Empty;
	}

	static bool Compare (MethodDefinition m1, MethodDefinition m2)
	{
		if (m1.Name != m2.Name)
			return false;

		if (m1.Parameters.Count != m2.Parameters.Count)
			return false;

		for (int i = 0; i < m1.Parameters.Count; i++) {
			ParameterDefinition p1 = m1.Parameters [i];
			ParameterDefinition p2 = m2.Parameters [i];
			if (p1.ParameterType.FullName != p2.ParameterType.FullName)
				return false;
		}
		return (m1.ReturnType.ReturnType.FullName == m2.ReturnType.ReturnType.FullName);
	}

	static void ProcessMethod (MethodDefinition method)
	{
		string comment = null;

		// p/invoke methods needs to be [SecurityCritical] to be executed, 
		// unless they are inside a type named "SafeNativeMethods"
		bool sc = (method.IsPInvokeImpl && (method.DeclaringType.Name != "SafeNativeMethods"));
		if (sc) {
			comment = "p/invoke declaration";
		}

		if (!sc) {
			comment = CheckVerifiability (method);
			sc = !String.IsNullOrEmpty (comment);
		}

		// skip signature check for visible API (we get them from find-sc)
		if (!sc && method.HasParameters && !method.IsVisible ()) {
			// compilers will add public stuff like: System.Action`1::.ctor(System.Object,System.IntPtr)
			if (!method.IsConstructor || (method.DeclaringType as TypeDefinition).BaseType.FullName != "System.MulticastDelegate") {
				foreach (ParameterDefinition p in method.Parameters) {
					if (!CheckType (p.ParameterType)) {
						sc = true;
						comment = String.Format ("using '{0}' as a parameter type", p.ParameterType.FullName);
						break;
					}
				}
			}
		}

		// skip signature check for visible API (we get them from find-sc)
		if (!sc && !method.IsVisible ()) {
			TypeReference rtype = method.ReturnType.ReturnType;
			if (!CheckType (rtype)) {
				sc = true;
				comment = String.Format ("using '{0}' as return type", rtype.FullName);
			}
		}

		// check if this method implements an interface where the corresponding member
		// is [SecurityCritical]
		TypeDefinition type = method.DeclaringType;
		if (!sc && type.HasInterfaces) {
			foreach (TypeReference intf in type.Interfaces) {
				TypeDefinition td = intf.Resolve ();
				if (td == null || !td.HasMethods)
					continue;
				foreach (MethodDefinition im in td.Methods) {
					if (im.IsSecurityCritical ()) {
						if (Compare (method, im)) {
							sc = true;
							comment = String.Format ("implements '{0}'.", im);
						}
					}
				}
			}
		}

		// if we're overriding a [SecurityCritical] method then we must be one too! or
		// if we are [SecurityCritical] then the base method needs to be too!
		if (method.IsVirtual && !method.IsAbstract) {
			TypeReference tr = method.DeclaringType.BaseType;
			if (tr != null) {
				TypeDefinition td = tr.Resolve ();
				if (td != null) {
					foreach (MethodDefinition bm in td.Methods) {
						if (Compare (method, bm)) {
							if (!sc) {
								if (bm.IsSecurityCritical ()) {
									sc = true;
									comment = String.Format ("overrides '{0}'.", bm);
								}
							} else {
								if (!bm.IsSecurityCritical ()) {
									string bms = bm.ToString ();
									if (!methods.ContainsKey (bms)) {
										comment = String.Format ("Promoting {0}base method to [SecurityCritical] because of '{1}'.", 
											bm.IsVisible () ? "[VISIBLE] " : String.Empty, method);
										methods.Add (bms, comment);
									}
								}
							}							
						}
					}
				}
			}
		} else {
			// note: we don't want to break the override rules above (resulting in TypeLoadException)
			// an icall that is NOT part of the visible API is considered as critical (like a p/invoke)
			if (method.IsInternalCall && !method.IsVisible ()) {
				sc = true;
				comment = "internal call";
			}
		}

		if (sc) {
			// note: add a warning on visible API since adding [SecurityCritical]
			// on "new" visible API would introduce incompatibility (so this needs
			// to be reviewed).
			if (method.IsVisible ())
				comment = "[VISIBLE] " + comment;
			methods.Add (method.GetFullName (), comment);
		}

		// if this method is [SecurityCritical] (already or because we determined it should be)
		// and implements an interface then some interface members may needs to be [SecurityCritical] 
		// too or a TypeLoadException will be thrown 
		// e.g. AppDomain.add_AssemblyResolve versus _AppDomain
		if (sc || method.IsSecurityCritical ()) {
			if (type.HasInterfaces) {
				foreach (TypeReference intf in type.Interfaces) {
					TypeDefinition td = intf.Resolve ();
					if (td == null || !td.HasMethods)
						continue;
					foreach (MethodDefinition im in td.Methods) {
						// note: in this case we don't care if the method is indirectly critical (e.g.via its type)
						if (Compare (method, im) && !type.IsSecurityCritical ()) {
							string ims = im.GetFullName ();
							// only add this if it's not something we already found before
							if (!methods.ContainsKey (ims)) {
								comment = String.Format ("Promoting {0}interface member to [SecurityCritical] because of '{1}'.", 
									im.IsVisible () ? "[VISIBLE] " : String.Empty, method);
								methods.Add (ims, comment);
							}
						}
					}
				}
			}
		}
	}

	static void ProcessType (TypeDefinition type)
	{
		if (type.HasConstructors) {
			foreach (MethodDefinition ctor in type.Constructors) {
				ProcessMethod (ctor);
			}
		}
		if (type.HasMethods) {
			foreach (MethodDefinition method in type.Methods) {
				ProcessMethod (method);
			}
		}
	}

	static void ProcessAssembly (AssemblyDefinition assembly)
	{
		foreach (ModuleDefinition module in assembly.Modules) {
			foreach (TypeDefinition type in module.Types) {
				ProcessType (type);
			}
		}
	}

	static int Main (string [] args)
	{
		int input;
		string output;

		switch (args.Length) {
		case 0:
			Console.WriteLine ("Usage: detect-sc input-dir [input-dir [...]] [output-dir]");
			return 1;
		case 1:
			output = args [0];
			input = 1;
			break;
		default:
			input = args.Length - 1;
			output = args [input];
			break;
		}

		foreach (string assembly in PlatformCode.Assemblies) {
			Console.Write ("{0}.dll:", assembly);

			string fullpath = null;
			for (int i = 0; i < input; i++) {
				fullpath = Path.Combine (args [i], assembly) + ".dll";
				if (File.Exists (fullpath))
					break;

				fullpath = null;
			}

			if (fullpath == null) {
				Console.WriteLine (" NOT FOUND!", assembly);
				continue;
			}

			AssemblyDefinition ad = AssemblyFactory.GetAssembly (fullpath);
			(ad.Resolver as BaseAssemblyResolver).AddSearchDirectory (args [0]);
			ProcessAssembly (ad);

			string outfile = Path.Combine (output, assembly) + ".auto.sc";
			using (StreamWriter sw = new StreamWriter (outfile)) {
				sw.WriteLine ("# [SecurityCritical] needed to execute code inside '{0}'.", ad.Name.FullName);
				sw.WriteLine ("# {0} methods needs to be decorated.", methods.Count);
				sw.WriteLine ();
				Console.Write (" {0} methods", methods.Count);
				foreach (KeyValuePair<string, string> kvp in methods) {
					sw.WriteLine ("# {0}", kvp.Value);
					sw.WriteLine ("+SC-M: {0}", kvp.Key);
					sw.WriteLine ();
				}
			}

			methods.Clear ();
			Console.WriteLine (".");
		}

		return 0;
	}
}
