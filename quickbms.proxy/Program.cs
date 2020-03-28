using JKang.IpcServiceFramework;
using Microsoft.Extensions.DependencyInjection;
using System;
using System.Net;

namespace QuickBms
{
    public class Program
    {
        public static void Main()
        {
            NativeMethods.Touch();

            // configure DI
            var services = ConfigureServices(new ServiceCollection());

            // build and run service host
            new IpcServiceHostBuilder(services.BuildServiceProvider())
                .AddNamedPipeEndpoint<IBmsService>(name: "endpoint1", pipeName: "pipeName")
                .AddTcpEndpoint<IBmsService>(name: "endpoint2", ipEndpoint: IPAddress.Loopback, port: 45684)
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
