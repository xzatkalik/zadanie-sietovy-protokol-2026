// See https://aka.ms/new-console-template for more information
//Console.WriteLine("Hello, World!");
//
//
//

using System;
using System.Net.Sockets;
using System.Text;

class Program
{
    const string SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 9000;

    static void Main()
    {
        try
        {
            using (TcpClient client = new TcpClient())
            {
                client.Connect(SERVER_IP, SERVER_PORT);
                Console.WriteLine($"Pripojené na {SERVER_IP}:{SERVER_PORT}");

                using (NetworkStream stream = client.GetStream())
                {
                    // Tu implementovat protokol
                }

                Console.WriteLine("Spojenie zatvorené");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Chyba: {ex.Message}");
        }
    }
}





