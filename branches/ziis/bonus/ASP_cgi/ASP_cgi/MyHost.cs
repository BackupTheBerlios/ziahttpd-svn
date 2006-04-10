
using System;
using System.IO;
using System.Web;
using System.Web.Hosting;
using System.Diagnostics;

namespace ASP_cgi
{	
	public class MyHost : MarshalByRefObject
	{
		public void CreateHtmlPage(String webPage, 
		                           String query)
		{
			SimpleWorkerRequest swr = new SimpleWorkerRequest(webPage,
			                                                  query,
			                                                  Console.Out);
			HttpRuntime.ProcessRequest(swr);
		}
	}
}
