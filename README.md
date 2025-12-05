# RebuiltSimon
Speedrun tool for Cry of Fear
<img width="923" height="394" alt="f7HbBOD" src="https://github.com/user-attachments/assets/449fe4fd-794c-4765-9452-d44704daf9cc" />

## Building
```sh
cmake -B build -A Win32
cmake --build build --config Release
```

## Usage
There are 2 versions of this tool you can use

### CoF Manager version
1. Install [CoF_Manager](https://github.com/cd-n0/CoF_Manager)
2. Copy or move `RebuiltSimonCoFManager.dll` to `Cry of Fear\cryoffear\addons` (If there is no addons folder inside `cryoffear` create it yourself)

### Injected version (Used for development)
Inject `RebuiltSimon.dll` to `cof.exe` with `injector.exe`, or with your favorite DLL injector
The injected version also opens up a console and has an extra command `rs_unload` that unloads the DLL

## Contributing
Pull requests are welcome. For major changes, please open an issue first to
discuss what you would like to change.

# Credits to:
- Kohtep for CoF Manager that helped kickstart this project
- Tsuda Kageyu for creating MinHook
- FWGS team for creating a GoldSrc compatible SDK and Xash3D
