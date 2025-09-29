# RebuiltSimon
Speedrun tool for Cry of Fear

## Building
```sh
cmake -B build -A Win32
cmake --build build --config Release
```

## Usage
There are 2 versions of this tool you can use

### CoF Manager version
1. Install [CoF_Manager](https://github.com/kohtep/CoF_Manager)
2. Copy/Move `RebuiltSimonCoFManager.dll` to `cryoffear/addons`

### Injected version (Used for development)
Inject `RebuiltSimon.dll` to `cof.exe` with your favorite injector
The injected version also opens up a console and has an extra command rs_unload that unloads the dll

## Contributing
Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

# Credits to:
- Kohtep for CoF Manager that helped kickstart this project
- Tsuda Kageyu for creating MinHook
- FWGS team for creating a GoldSrc compatible SDK and Xash3D