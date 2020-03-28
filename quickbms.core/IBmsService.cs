namespace QuickBms
{
    // https://nondisplayable.ca/2015/01/28/remoting-between-32-bit-and-64-bit-dotnet.html
    // https://stackoverflow.com/questions/48522849/what-replaces-wcf-in-net-core
    public interface IBmsService
    {
        float AddFloat(float x, float y);
    }
}
