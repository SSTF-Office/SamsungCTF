using System;
using System.Text;
using System.Security.Cryptography;

/* Answer: "C4E35E (^KE" */

namespace ConsoleApp1
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            int num = 3;
            int pad = 2;
            string value = "04befa704259028fe18c2763e1106e7d28104b1f6466010208e1c975b6b47d50de9001f3b15feb3dac45816f645f3aa42dfc7760e3fa09fbf2a8a3de85a9eede7b598a595430b4add2f939d8ff9a51ae30d2e9e2188d0e76eae70b3c7db13315a7052319cdb35ea5a1805529c71dcbc3547c2d638d11582388d5d8b46bba3f61679431ca4a677b9405974a4ed20ff43c636b87e3260d93503daf8d4090f310e51ab36a6268e6fffd304e69dbc82b5cef9b89ae9e904d716e006494462ec982cc";
            Console.WriteLine("Hello!");
            Console.WriteLine("Can you guess one of my favorite things?");
            string text;
            string input_str;
            for (; ; )
            {
                Console.Write("Answer: ");
                text = Console.ReadLine();
                input_str = text;
                if (text.Equals("Quit"))
                {
                    return;
                }

                text = text.PadRight(num + text.Length);
                if (Program.HashAll(text, num, pad).Equals(value))
                {
                    break;
                }
                Console.WriteLine("Not correct!");
            }
            Console.WriteLine("Congratz! That's a correct answer\n");
            Console.WriteLine("SCTF{" + Program.ComputeSha256Hash(input_str) + "}");
            Console.ReadKey();
        }

        private static string HashAll(string ans, int len, int pad)
        {
            int num = 0;
            StringBuilder stringBuilder = new StringBuilder();
            while (num + len - 1 < ans.Length)
            {
                string chars = ans.Substring(num, len);
                stringBuilder.Append(ComputeSha256Hash(chars));
                num += pad;
            }
            return stringBuilder.ToString();
        }

        private static string ComputeSha256Hash(string rawData)
        {
            using (SHA256 sha256Hash = SHA256.Create())
            {            
                byte[] bytes = sha256Hash.ComputeHash(Encoding.UTF8.GetBytes(rawData));

                StringBuilder builder = new StringBuilder();
                for (int i = 0; i < bytes.Length; i++)
                {
                    builder.Append(bytes[i].ToString("x2"));
                }
                return builder.ToString();
            }
        }
    }
}
