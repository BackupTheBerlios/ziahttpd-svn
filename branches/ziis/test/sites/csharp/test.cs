using System;

namespace Cgi
{ 
    public class Cgi
    { 
        public object main()
        {
            Console.WriteLine("<html><body>");
            Console.WriteLine("Hello from C#<br><br>");
            for (int i = 0; i < 10; i++ )
                Console.WriteLine("salut ca roule<br>");
            Console.WriteLine("</html></body>");
            return null; 
        } 
    } 
}