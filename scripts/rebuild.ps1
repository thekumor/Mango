$p = Start-Process Powershell.exe -ArgumentList '-ExecutionPolicy remotesigned -File scripts/clear.ps1' -PassThru
$p.WaitForExit()

Start-Process Powershell.exe -ArgumentList '-ExecutionPolicy remotesigned -File scripts/build.ps1' -PassThru