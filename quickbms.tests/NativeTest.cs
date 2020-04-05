using NUnit.Framework;

namespace QuickBms.Tests
{
    public class NativeTest
    {
        [Test]
        public void TestLoad()
        {
            NativeMethods.Touch();
        }

        //quickbms -l witcher3.bms "D:\Program Files (x86)\GOG Galaxy\Games\The Witcher 3 Wild Hunt\content\*"
    }
}
