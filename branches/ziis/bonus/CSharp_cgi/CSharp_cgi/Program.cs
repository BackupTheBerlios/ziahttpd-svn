using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using System.CodeDom.Compiler;
using Microsoft.CSharp;
using System.Reflection;
using System.IO;

namespace CSharp_cgi
{
    class Program
    {
        static void Main(string[] args)
        {
            string script;
            string filename;


            Console.WriteLine("Content-type : text/html");

            if (args.Length == 0)
            {
                Console.WriteLine("Status : 404");
                Console.WriteLine("");
                Console.WriteLine("This application must take an arguement, check your configuration file");
                return;
            }
            filename = args[0];
            try
            {
                StreamReader sr = new StreamReader(filename);
                Console.WriteLine("");
                script = sr.ReadToEnd();
                Eval(script);
            }
            catch (Exception)
            {
                Console.WriteLine("Status : 404");
                Console.WriteLine("");
                Console.WriteLine("File not found");
                return;
            }
        }

        public static object Eval(string sCSCode)
        {
            CSharpCodeProvider c = new CSharpCodeProvider();
            ICodeCompiler icc = (ICodeCompiler)c.CreateCompiler();
            CompilerParameters cp = new CompilerParameters();

            cp.ReferencedAssemblies.Add("system.dll");
            cp.ReferencedAssemblies.Add("system.xml.dll");
            cp.ReferencedAssemblies.Add("system.data.dll");
            cp.ReferencedAssemblies.Add("system.windows.forms.dll");
            cp.ReferencedAssemblies.Add("system.drawing.dll");

            cp.CompilerOptions = "/t:library";
            cp.GenerateInMemory = true;
            StringBuilder sb = new StringBuilder("");

            sb.Append(sCSCode);

            CompilerResults cr = icc.CompileAssemblyFromSource(cp, sb.ToString());
            if (cr.Errors.Count > 0)
            {
                Console.Write("ERROR: " + cr.Errors[0].ErrorText, "Error evaluating cs code");
                return null;
            }

            System.Reflection.Assembly a = cr.CompiledAssembly;
            object o = a.CreateInstance("Cgi.Cgi");
            Type t = o.GetType();
            MethodInfo mi = t.GetMethod("main");
            object s = mi.Invoke(o, null);
            return s;
        }
    }
}
