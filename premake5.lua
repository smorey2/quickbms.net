if os.ishost "windows" then

    -- Windows
    newaction
    {
        trigger     = "solution",
        description = "Open the QuickBMS.Net solution",
        execute = function ()
            os.execute "start QuickBMS.Net.sln"
        end
    }

    newaction
    {
        trigger     = "native",
        description = "Build the native QuickBMS library",
        execute = function ()
            os.execute
"mkdir _build32 & pushd _build32 \z
&& cmake -G \"Ninja\" ..\\ \z
&& popd \z
&& cmake --build _build32"
        end
    }

    newaction
    {
        trigger     = "native_",
        description = "Build the native QuickBMS library",
        execute = function ()
            os.execute "mkdir _build32 & pushd _build32 \z
&& cmake -G \"Visual Studio 15 2017\" -DBITNESS=\"x86\" ..\\ \z
&& popd \z
&& mkdir _build64 & pushd _build64 \z
&& cmake -G \"Visual Studio 15 2017 Win64\" -DBITNESS=\"x64\" ..\\ \z
&& popd \z
&& cmake --build _build32 --config Release \z
&& copy _build32\\Release\\QuickBMS.exe QuickBMS.Net\\x86 \z
&& cmake --build _build64 --config Release \z
&& copy _build64\\Release\\QuickBMS.exe QuickBMS.Net\\x64"
        end
    }

else

     -- MacOSX and Linux.
    
     newaction
     {
         trigger     = "solution",
         description = "Open the QuickBMS.Net solution",
         execute = function ()
         end
     }
 
     newaction
     {
         trigger     = "loc",
         description = "Count lines of code",
         execute = function ()
             os.execute "wc -l *.cs"
         end
     }
     
end

newaction
{
    trigger     = "build",
    description = "Build QuickBMS.Net",
    execute = function ()
        os.execute "dotnet build QuickBMS.Net/QuickBMS.Net.csproj"
    end
}

newaction
{
    trigger     = "test",
    description = "Build and run all unit tests",
    execute = function ()
        -- os.execute "premake5 build"
        os.execute "dotnet test QuickBMS.Tests/QuickBMS.Tests.csproj"
    end
}

newaction
{
    trigger     = "pack",
    description = "Package and run all unit tests",
    execute = function ()
        os.execute "dotnet pack QuickBMS.Net/QuickBMS.Net.csproj --output ../nupkgs --include-source"
    end
}

newaction
{
    trigger     = "publish",
    description = "Package and publish nuget package",
    execute = function ()
        apikey = os.getenv('NUGET_APIKEY')
        os.execute "premake5 pack"
        os.execute( "dotnet nuget push nupkgs/**/QuickBMS.*.nupkg --api-key " .. apikey .. " --source https://api.nuget.org/v3/index.json" )
    end
}


newaction
{
    trigger     = "clean",
    description = "Clean all build files and output",
    execute = function ()
        files_to_delete = 
        {
            "*.make",
            "*.zip",
            "*.tar.gz",
            "*.db",
            "*.opendb"
        }
        directories_to_delete = 
        {
            "obj",
            "ipch",
            "bin",
            "nupkgs",
            ".vs",
            "Debug",
            "Release",
            "release"
        }
        for i,v in ipairs( directories_to_delete ) do
          os.rmdir( v )
        end
        if not os.ishost "windows" then
            os.execute "find . -name .DS_Store -delete"
            for i,v in ipairs( files_to_delete ) do
              os.execute( "rm -f " .. v )
            end
        else
            for i,v in ipairs( files_to_delete ) do
              os.execute( "del /F /Q  " .. v )
            end
        end

    end
}
