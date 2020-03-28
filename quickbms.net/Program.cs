using JKang.IpcServiceFramework;
using System.Threading.Tasks;

namespace QuickBms
{
    public class Program
    {
        public static void Main()
        {
            var client = new IpcServiceClientBuilder<IBmsService>()
                .UseNamedPipe("pipeName") // or .UseTcp(IPAddress.Loopback, 45684) to invoke using TCP
                .Build();

            GoAsync(client).Wait();
        }

        static async Task GoAsync(IpcServiceClient<IBmsService> client)
        {
            var result = await client.InvokeAsync(x => x.AddFloat(1.23f, 4.56f));
        }
    }
}
