# This script send the Wake On Lan command to the specified MAC Address
# Run this steps on windows powershell.
# Powershell does not run ps1 script on doubleclick.
# You need to copy and paste the commands on a powershell terminal
# Change the mac address as per your needs

$Mac = "18:A9:05:B9:DB:76"
$MacByteArray = $Mac -split "[:-]" | ForEach-Object { [Byte] "0x$_"}
[Byte[]] $MagicPacket = (,0xFF * 6) + ($MacByteArray  * 16)
$UdpClient = New-Object System.Net.Sockets.UdpClient
$UdpClient.Connect(([System.Net.IPAddress]::Broadcast),7)
$UdpClient.Send($MagicPacket,$MagicPacket.Length)
$UdpClient.Close()



