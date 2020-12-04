# TDLib Native Sciter Extension

Simple [Sciter](https://sciter.com) native extension for TDLib's JSON interface. Tested on Windows with Visual Studio 2019, possible (but haven't tested yet) to work on Linux.

## Prerequisites to build

* CMake
* Visual Studio (on Windows) or any other CMake supported compiler
* TDLib (as CMake package)
* Sciter SDK

## Build process

This instruction assumes the following:
* CMake is installed and added to your PATH environment variable
* Commands are executed in "x86 Native Tools Command Prompt for VS 2019"
* TDLib is installed in C:\TDLib
* Sciter SDK is unpacked at C:\sciter
* This repo is cloned to C:\TDLibNSE
* Build files will be located at C:\TDLibNSE\build

Generate build files (starting from C:\TDLibNSE):
```
mkdir build
cd build
cmake -A Win32 -DTd_DIR=C:\TDLib\lib\cmake\Td -DSCITER_INCLUDE_DIR=C:\sciter\include ..\TDLibNSE
```

Omit `-A Win32` to use your default system architecture.

Omit any of `-DTD_DIR` and `-DSCITER_INCLUDE_DIR` (or all) to search packages in default paths.

You can also add `-DUSE_STATIC_TDJSON=ON` to `cmake` arguments if you want it to link extension against static library instead of dynamic.


Build (starting from C:\TDLibNSE\build):
```
cmake --build . --clean-first --config Release
```

Omit `--clean-first` if you don't want to clean project before building.

Omit `--config Release` if you wan't to build default configuration `Debug`. You can also specify it explicitly: `--config Debug`.


## Using the library

On Windows you need to following files to be put near scapp.exe or to be found in system paths:
* TDLibNSE.dll (your built file, can be found in C:\TDLibNSE\build\Release or C:\TDLibNSE\build\Debug)
* tdjson.dll (TDLib with JSON interface, can be found at C:\TDLib\bin)
* libcrypto-1_1.dll (TDLib prereqisites)
* libssl-1_1.dll (TDLib prereqisites)
* zlib1.dll (TDLib prereqisites)

If you have installed TDLib prerequisites from vcpkg and installed vcpkg to C:\vcpkg, these files can be found at C:\vcpkg\installed\x86-windows\bin (or vcpkg\installed\x64-windows\bin for x64).

After that, you can include it in your Sciter HTML file as:
```
include library "TDLibNSE";
```
or
```
const Td = include library "TDLibNSE";
```
or even
```
namespace Td {
	const tdlib = include library "TDLibNSE";
}
```

## API

API in TDLibNSE is pretty much same as TDLib new JSON interface.

* `int TDLib.createClient()` – creates new TDLib client and returns its ID.
* `void TDLib.send(int client_id, string request)` – sends request JSON encoded string to specified client_id.
* `string TDLib.receive(double timeout)` – polls for new updates from clients for timeout, returns JSON encoded string of response.
* `void TDLib.startReceiveThread(double timeout, function callback)` – polls for new updates from clients for timeout in separate thread, calls `callback` with returns JSON encoded string of response.
  * Callback signature is `int callback(string result)`.
    * Return `-1` in callback to stop thread.
    * Return `0` in callback to specify no delay.
    * Return a positive number to specify a delay in milliseconds.
  * When thread is started, first JSON received will be `{"tdlibnse":"receive_thread_started"}`.
* `void TDLib.stopReceiveThread()` – stops polling for updates in thread.
  * When thread is stopped, last JSON received will be `{"tdlibnse":"receive_thread_stopped"}`.
* `bool TDLib.receiveThreadRunning()` – returns `true` if polling thread is running.
* `string TDLib.execute(string request)` – sends request JSON encoded string to TDLib, returns JSON encoded string of response.

## Recommendations
* For some reason, if `setLogVerbosityLevel` is not set, TDLib may eat your CPU even if you specified a delay in receive thread.

## Example usage

```html
<html>
<head>
  <title>TDLib Test</title>
  <style>
    #out {
      font: monospace;
    }
  </style>
</head>
<script type="text/tiscript">
  include library "TDLibNSE";

  var testRequest = {
      "@type": "getTextEntities",
      "text": "@telegram /test_command https://telegram.org telegram.me",
      "@extra": ["5", 7.0]
  };

  event click $(#test) {
    if (TDLib) {
        var j = JSON.parse(TDLib.execute(JSON.stringify(testRequest)));
        $(#out).text = JSON.stringify(j);
    } else {
        $(#out).text = "Couldn't load TDLib.";
    }
  }
</script>
<body>
  <button#test>Test TDLib</button>
  <text#out />
</body>
</html>
```

Request structure was taken from TDLib's [Python example](https://github.com/tdlib/td/blob/master/example/python/tdjson_example.py).
