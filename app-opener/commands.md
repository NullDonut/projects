## Packages List
```
adb shell pm list packages
```

## Open App in popup window 

```
adb shell am force-stop com.package.name
adb shell am start -n com.termux.x11/com.termux.x11.MainActivity --windowingMode 5
```
