using System;
using System.IO;
using System.Web;
using System.Web.Hosting;

namespace ASP_cgi
{
    class Program
    {
        static MyHost m_host = null; 
        public  static void Main(String[] args)
        {
            string filename;
            string filenametmp;
            if (args.Length == 0)
            {
                Console.WriteLine("Content-type : text/html");
                Console.WriteLine("");
                Console.WriteLine("Internal Error");
                return ;
            }
            Console.WriteLine("Content-type : text/html");
            Console.WriteLine("");
            filename = args[0];
            filenametmp = filename.Replace('/', '\\');
            filename = filenametmp;
            filenametmp = filename.Replace("\\\\", "\\");
            filename = filenametmp;
            filenametmp = filename.Substring(2, filename.Length - 2);
            filename = filenametmp;
            int i;

            for (i = 0; i < filename.Length; i++)
            {
                if ((filename[i] >= 'a') && (filename[i] <= 'z'))
                    break;
            }
            filenametmp = filename.Substring(i, filename.Length - i);
            filename = filenametmp;
            InitBrowserComponent(filename);
        }

        public  static void InitBrowserComponent(Object sender)
        {
            string webPage = null;
            string strFileExt = null;
            string[] strExtList;
            webPage = (string)sender;

            if (webPage == null)
            {
                return;
            }

            if (m_host == null)
            {
                m_host = (MyHost)ApplicationHost.CreateApplicationHost(
                                                           typeof(MyHost),
                                                           "/",
                                                           Environment.CurrentDirectory);
            }
            strExtList = webPage.Split('.');
            if (strExtList != null)
            {
                strFileExt = strExtList[strExtList.Length - 1];
            }

            if (strFileExt != null && strFileExt.ToUpper().Equals("ASPX"))
            {
                string query = System.Environment.GetEnvironmentVariable("QUERY_STRING");
                m_host.CreateHtmlPage(webPage, query);
            }
        }
    }
}