#define CORECLR
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace QuickBms
{
    #region NativeLibrary

    public interface INativeLibrary : IDisposable
    {
        T MarshalStructure<T>(string name);
        T[] MarshalToPtrArray<T>(string name, int count);
    }

    public static class NativeLibrary
    {
        internal sealed class WindowsNativeLibrary : INativeLibrary
        {
            readonly IntPtr _LibarayHandle;
            bool _IsDisposed = false;

            public WindowsNativeLibrary(string name)
            {
                var dllPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, Environment.Is64BitProcess ? "x64" : "x86");
                Environment.SetEnvironmentVariable("PATH", $"{Environment.GetEnvironmentVariable("PATH")};{dllPath}");
                _LibarayHandle = LoadLibrary(name);
                if (_LibarayHandle == IntPtr.Zero)
                    throw new InvalidOperationException($"library {name} not found");
            }

            public void Dispose()
            {
                if (_IsDisposed)
                    return;
                FreeLibrary(_LibarayHandle);
                _IsDisposed = true;
            }

            public T MarshalStructure<T>(string name)
            {
                if (_IsDisposed)
                    throw new ObjectDisposedException("UnmanagedLibrary");
                var ptr = GetProcAddress(_LibarayHandle, name);
                return ptr != IntPtr.Zero ? Marshal.PtrToStructure<T>(ptr) : throw new InvalidOperationException($"function {name} not found");
            }

            public T[] MarshalToPtrArray<T>(string name, int count)
            {
                if (_IsDisposed)
                    throw new ObjectDisposedException("UnmanagedLibrary");
                var ptr = GetProcAddress(_LibarayHandle, name);
                if (ptr == IntPtr.Zero)
                    return new T[0];
                var r = new T[count];
                var typeofT = typeof(T);
                if (typeofT == typeof(int)) Marshal.Copy(ptr, (int[])(object)r, 0, r.Length);
                else throw new ArgumentOutOfRangeException(nameof(T), typeofT.Name);
                return r;
            }

            ~WindowsNativeLibrary() { Dispose(); }

            [DllImport("kernel32.dll")] static extern IntPtr LoadLibrary(string fileName);
            [DllImport("kernel32.dll")] static extern int FreeLibrary(IntPtr handle);
            [DllImport("kernel32.dll")] static extern IntPtr GetProcAddress(IntPtr handle, string procedureName);
        }

        internal sealed class LinuxNativeLibrary : INativeLibrary
        {
            readonly IntPtr _LibarayHandle;
            const int RTLD_NOW = 2;
            bool _IsDisposed = false;

            public LinuxNativeLibrary(string name)
            {
                var library = (IntPtr.Size == 8 ? $"x64\\{name}" : $"x86\\{name}");
                _LibarayHandle = dlopen(library, RTLD_NOW);
                if (_LibarayHandle == IntPtr.Zero)
                    throw new InvalidOperationException($"library {name} not found");
            }

            public void Dispose()
            {
                if (_IsDisposed)
                    return;
                dlclose(_LibarayHandle);
                _IsDisposed = true;
            }

            public T MarshalStructure<T>(string name)
            {
                if (_IsDisposed)
                    throw new ObjectDisposedException("UnmanagedLibrary");
                // clear previous errors if any
                dlerror();
                var ptr = dlsym(_LibarayHandle, name);
                var errPtr = dlerror();
                if (errPtr != IntPtr.Zero)
                    throw new Exception("dlsym: " + Marshal.PtrToStringAnsi(errPtr));
                return Marshal.PtrToStructure<T>(ptr);
            }

            public T[] MarshalToPtrArray<T>(string name, int count)
            {
                if (_IsDisposed)
                    throw new ObjectDisposedException("UnmanagedLibrary");
                // clear previous errors if any
                dlerror();
                var ptr = dlsym(_LibarayHandle, name);
                var errPtr = dlerror();
                if (errPtr != IntPtr.Zero)
                    throw new Exception("dlsym: " + Marshal.PtrToStringAnsi(errPtr));
                if (ptr == IntPtr.Zero)
                    return new T[0];
                var r = new T[count];
                var typeofT = typeof(T);
                if (typeofT == typeof(int)) Marshal.Copy(ptr, (int[])(object)r, 0, r.Length);
                else throw new ArgumentOutOfRangeException(nameof(T), typeofT.Name);
                return r;
            }

            ~LinuxNativeLibrary() { Dispose(); }

            [DllImport("libdl.so")] static extern IntPtr dlopen(string fileName, int flags);
            [DllImport("libdl.so")] static extern IntPtr dlsym(IntPtr handle, string symbol);
            [DllImport("libdl.so")] static extern int dlclose(IntPtr handle);
            [DllImport("libdl.so")] static extern IntPtr dlerror();
        }

        public static INativeLibrary Create(string name)
        {
#if CORECLR
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows)) return new WindowsNativeLibrary(name);
            else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux)) return new LinuxNativeLibrary(name);
            else throw new NotImplementedException("Unmanaged library loading is not implemented on this platform");
#else
            return new WindowsNativeLibrary(name);
#endif
        }
    }

    public static class Marshal2
    {
        public static TStruct ToMarshaled<TStruct>(this IntPtr ptr)
           => ptr != IntPtr.Zero ? Marshal.PtrToStructure<TStruct>(ptr) : default(TStruct);
        public static void MarshaledToPtr<TStruct>(this IntPtr ptr, TStruct value, bool fDeleteOld)
        {
            if (ptr == IntPtr.Zero)
                throw new Exception();
            Marshal.StructureToPtr(value, ptr, fDeleteOld);
        }

        //public static TStruct[] ToMarshaledArray<TStruct>(this IntPtr ptr, int count)
        //    => ptr != IntPtr.Zero ? new[] { default(TStruct) } : null;

        public static T ToMarshaledObject<T, TStruct>(this IntPtr ptr)
            where T : MarshaledObject<TStruct>, new()
            => ptr != IntPtr.Zero ? new T { Ptr = ptr } : null;
        public static void MarshaledObjectToPtr<T, TStruct>(this IntPtr ptr, T value, bool fDeleteOld)
            where T : MarshaledObject<TStruct>
        {
            if (ptr == IntPtr.Zero)
                throw new Exception();
            Marshal.StructureToPtr(value.Value, ptr, fDeleteOld);
        }

        public static string ReadStringAscii(IntPtr ptr)
        {
            ptr = Marshal.ReadIntPtr(ptr);
            return ptr != IntPtr.Zero ? Marshal.PtrToStringAnsi(ptr) : null;
        }
        public static void WriteStringAscii(IntPtr ptr, string value) => Marshal.WriteIntPtr(ptr, value != null ? Marshal.StringToHGlobalAnsi(value) : IntPtr.Zero);

        public static string ReadDStringAscii(IntPtr ptr)
        {
            ptr = Marshal.ReadIntPtr(ptr);
            if (ptr == IntPtr.Zero)
                return null;
            int length;
            if (Environment.Is64BitProcess) { length = (int)Marshal.ReadInt64(ptr); ptr += sizeof(long); }
            else { length = Marshal.ReadInt32(ptr); ptr += sizeof(int); }
            return length != 0 ? Marshal.PtrToStringAnsi(ptr, length) : string.Empty; ;
        }

        public static TStruct ReadMarshaled<TStruct>(IntPtr ptr)
        {
            ptr = Marshal.ReadIntPtr(ptr);
            return ptr != IntPtr.Zero ? Marshal.PtrToStructure<TStruct>(ptr) : default(TStruct);
        }

        public static TStruct[] ReadMarshaledArray<TStruct>(IntPtr ptr, int count)
        {
            ptr = Marshal.ReadIntPtr(ptr);
            if (ptr == IntPtr.Zero)
                return new TStruct[0];
            var r = new TStruct[count];
            if (typeof(TStruct) == typeof(byte)) Marshal.Copy(ptr, (byte[])(object)r, 0, count);
            else throw new ArgumentOutOfRangeException(nameof(TStruct));
            return r;
        }

        public static T ReadMarshaledObject<T, TStruct>(IntPtr ptr)
            where T : MarshaledObject<TStruct>, new()
        {
            ptr = Marshal.ReadIntPtr(ptr);
            return ptr != IntPtr.Zero ? new T { Ptr = ptr } : null;
        }
    }

    public class MarshaledObjectArrayAccessor<T, TStruct, TField, TFieldStruct>
        where T : MarshaledObject<TStruct>
        where TField : MarshaledObject<TFieldStruct>, new()
    {
        readonly int SizeOf;
        readonly IntPtr Ptr;
        public MarshaledObjectArrayAccessor(T parent, int fieldOffset)
        {
            SizeOf = Marshal.SizeOf<TFieldStruct>();
            Ptr = parent.Ptr + fieldOffset;
        }
        public TField this[int index]
        {
            get => Marshal2.ToMarshaledObject<TField, TFieldStruct>(Ptr + index * SizeOf);
            //set => Marshal2.MarshaledObjectToPtr<TField, TFieldStruct>(Ptr + index * SizeOf, value, false);
        }
    }

    public class MarshaledObject<TStruct> : IDisposable
    {
        public IntPtr Ptr;
        readonly Action<IntPtr> _free;

        public MarshaledObject() { } //this(arg => Marshal.AllocHGlobal(Marshal.SizeOf<TStruct>()), null, Marshal.FreeHGlobal) { }
        public MarshaledObject(Func<object, IntPtr> @new, object newArg, Action<IntPtr> free)
        {
            if (@new != null)
            {
                Ptr = @new(newArg);
                if (Ptr == IntPtr.Zero)
                    throw new ArgumentNullException(nameof(Ptr));
            }
            _free = free;
        }
        ~MarshaledObject() => Dispose();

        public void Dispose()
        {
            if (Ptr == IntPtr.Zero || _free == null)
                return;
            _free(Ptr);
            Ptr = IntPtr.Zero;
        }

        public TStruct Value => Marshal.PtrToStructure<TStruct>(Ptr);
        public void WithPtr(Action<IntPtr> action) => action(Ptr);
    }

    #endregion

    #region NativeMethods

    public static partial class NativeMethods
    {
        const string LibraryName = "quickbms.exe";
        static bool _disposed;
        //static readonly NativeFactory _factory = new NativeFactory(new LazyFactory(() => _disposed, typeof(NativeMethods).Name));
        static readonly INativeLibrary _library = NativeLibrary.Create(LibraryName);

        public static void Dispose()
        {
            if (_disposed)
                return;
            _library?.Dispose();
            _disposed = true;
        }

        public static void Touch() { var _ = _library; }

        public static T MarshalStructure<T>(string name) => _library.MarshalStructure<T>(name);
        public static T[] MarshalToPtrArray<T>(string name, int count) => _library.MarshalToPtrArray<T>(name, count);
    }

    #endregion

    #region Marshaler

    public class DStrMarshaler : ICustomMarshaler
    {
        /// <summary>
        /// All custom marshalers require a static factory method with this signature.
        /// </summary>
        /// <param name="cookie"></param>
        /// <returns></returns>
        public static ICustomMarshaler GetInstance(string cookie) => new DStrMarshaler();

        public object MarshalNativeToManaged(IntPtr pNativeData)
        {
            if (pNativeData == IntPtr.Zero)
                return null;
            int length;
            if (Environment.Is64BitProcess) { length = (int)Marshal.ReadInt64(pNativeData); pNativeData += sizeof(long); }
            else { length = Marshal.ReadInt32(pNativeData); pNativeData += sizeof(int); }
            var value = Marshal.PtrToStringAnsi(pNativeData, length);
            return value;
        }

        public IntPtr MarshalManagedToNative(object ManagedObj)
        {
            if (ManagedObj == null)
                return IntPtr.Zero;
            var value = (string)ManagedObj;
            var bytes = Encoding.ASCII.GetBytes(value);
            IntPtr ptr;
            if (Environment.Is64BitProcess)
            {
                ptr = Marshal.AllocHGlobal(sizeof(long) + value.Length);
                Marshal.ReadInt64(ptr, value.Length);
                Marshal.Copy(bytes, 0, ptr + sizeof(long), value.Length);
            }
            else
            {
                ptr = Marshal.AllocHGlobal(sizeof(int) + value.Length);
                Marshal.ReadInt32(ptr, value.Length);
                Marshal.Copy(bytes, 0, ptr + sizeof(int), value.Length);
            }
            return ptr;
        }

        public void CleanUpNativeData(IntPtr pNativeData) => Marshal.FreeHGlobal(pNativeData);

        public void CleanUpManagedData(object ManagedObj) { }

        public int GetNativeDataSize() => -1;
    }

    /// <summary>
    /// ArrayMarshaler
    /// </summary>
    /// <typeparam name="T"></typeparam>
    public class ArrayMarshaler<T> : ICustomMarshaler
    {
        /// <summary>
        /// All custom marshalers require a static factory method with this signature.
        /// </summary>
        /// <param name="cookie"></param>
        /// <returns></returns>
        public static ICustomMarshaler GetInstance(string cookie) => new ArrayMarshaler<T>();

        /// <summary>
        /// This is the function that builds the managed type - in this case, the managed array - from a pointer. You can just return null here if only sending the array as an in-parameter.
        /// </summary>
        /// <param name="pNativeData"></param>
        /// <returns></returns>
        public object MarshalNativeToManaged(IntPtr pNativeData)
        {
            // First, sanity check...
            if (pNativeData == IntPtr.Zero) return null;
            // Start by reading the size of the array ("Length" from your ABS_DATA struct)
            var length = Marshal.ReadInt32(pNativeData);
            // Create the managed array that will be returned
            var array = new T[length];
            // For efficiency, only compute the element size once
            var elSiz = Marshal.SizeOf<T>();
            // Populate the array
            for (int i = 0; i < length; i++)
                array[i] = Marshal.PtrToStructure<T>(pNativeData + sizeof(int) + (elSiz * i));
            // Alternate method, for arrays of primitive types only:
            // Marshal.Copy(pNativeData + sizeof(int), array, 0, length);
            return array;
        }

        /// <summary>
        /// This is the function that marshals your managed array to unmanaged memory. If you only ever marshal the array out, not in, you can return IntPtr.Zero
        /// </summary>
        /// <param name="ManagedObject"></param>
        /// <returns></returns>
        public IntPtr MarshalManagedToNative(object ManagedObject)
        {
            if (ManagedObject == null)
                return IntPtr.Zero;
            var array = (T[])ManagedObject;
            var elSiz = Marshal.SizeOf<T>();
            // Get the total size of unmanaged memory that is needed (length + elements)
            var size = sizeof(int) + (elSiz * array.Length);
            // Allocate unmanaged space. For COM, use Marshal.AllocCoTaskMem instead.
            var ptr = Marshal.AllocHGlobal(size);
            // Write the "Length" field first
            Marshal.WriteInt32(ptr, array.Length);
            // Write the array data
            for (int i = 0; i < array.Length; i++)
                Marshal.StructureToPtr<T>(array[i], ptr + sizeof(int) + (elSiz * i), false); // Newly-allocated space has no existing object, so the last param is false
            // If you're only using arrays of primitive types, you could use this instead:
            //Marshal.Copy(array, 0, ptr + sizeof(int), array.Length);
            return ptr;
        }

        /// <summary>
        /// This function is called after completing the call that required marshaling to unmanaged memory. You should use it to free any unmanaged memory you allocated.
        /// If you never consume unmanaged memory or other resources, do nothing here.
        /// </summary>
        /// <param name="pNativeData"></param>
        public void CleanUpNativeData(IntPtr pNativeData) => Marshal.FreeHGlobal(pNativeData); // Free the unmanaged memory. Use Marshal.FreeCoTaskMem if using COM.

        /// <summary>
        /// If, after marshaling from unmanaged to managed, you have anything that needs to be taken care of when you're done with the object, put it here. Garbage 
        /// collection will free the managed object, so I've left this function empty.
        /// </summary>
        /// <param name="ManagedObj"></param>
        public void CleanUpManagedData(object ManagedObj) { }

        /// <summary>
        /// This function is a lie. It looks like it should be impossible to get the right value - the whole problem is that the size of each array is variable!  
        /// - but in practice the runtime doesn't rely on this and may not even call it. The MSDN example returns -1; I'll try to be a little more realistic.
        /// </summary>
        /// <returns></returns>
        public int GetNativeDataSize() => sizeof(int) + Marshal.SizeOf<T>();
    }

    #endregion
}
