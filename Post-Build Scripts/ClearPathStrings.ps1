Param(
    [string]$filePath
)

$contentBytes = [IO.File]::ReadAllBytes($filePath)

$content = [System.Text.Encoding]::Default.GetString($contentBytes)

$pathRegex = '[A-Za-z]:\\(?:[^\\\/:*?"<>|\x00\r\n]+\\)*[^\\\/:*?"<>|\x00\r\n]*'

$matches = [regex]::Matches($content, $pathRegex)

foreach ($match in $matches) {
    $startIndex = $match.Index
    $length = $match.Length

    for ($i = $startIndex; $i -lt ($startIndex + $length); $i++) {
        $contentBytes[$i] = Get-Random -Minimum 33 -Maximum 127
    }
}

[IO.File]::WriteAllBytes($filePath, $contentBytes)