using JKang.IpcServiceFramework;
using Microsoft.Extensions.DependencyInjection;
using System.Net;
using System.Runtime.InteropServices;

namespace QuickBms
{
    public class Program
    {
        public static void Main()
        {
            NativeMethods.Touch();

            var test = NativeMethods.proxy_test(2);
            //var ver = NativeMethods.proxy_ver();
            var cmd = NativeMethods.proxy_script(); // "");

            // configure DI
            var services = ConfigureServices(new ServiceCollection());

            // build and run service host
            new IpcServiceHostBuilder(services.BuildServiceProvider())
                .AddNamedPipeEndpoint<IBmsService>(name: "endpoint1", pipeName: "pipeName")
                //.AddTcpEndpoint<IBmsService>(name: "endpoint2", ipEndpoint: IPAddress.Loopback, port: 45684)
                .Build()
                .Run();
        }

        static IServiceCollection ConfigureServices(IServiceCollection services)
        {
            return services
                .AddIpc(builder =>
                {
                    builder
                        .AddNamedPipe(options =>
                        {
                            options.ThreadCount = 2;
                        })
                        .AddService<IBmsService, BmsService>();
                });
        }
    }
}
