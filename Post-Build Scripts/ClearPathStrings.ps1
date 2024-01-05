Param(
    [string]$filePath
)

# Read the binary content as bytes
$contentBytes = [IO.File]::ReadAllBytes($filePath)

# Convert the byte array to a string for regex matching
$content = [System.Text.Encoding]::Default.GetString($contentBytes)

# Define a regex that matches typical file paths (e.g., 'C:\path\to\file.ext')
$pathRegex = '[A-Za-z]:\\(?:[^\\\/:*?"<>|\x00\r\n]+\\)*[^\\\/:*?"<>|\x00\r\n]*'

# Find matches in the content
$matches = [regex]::Matches($content, $pathRegex)

# Iterate through matches and replace with 1's
foreach ($match in $matches) {
    $startIndex = $match.Index
    $length = $match.Length

    # Replace with ASCII code for '1' (which is 49)
    for ($i = $startIndex; $i -lt ($startIndex + $length); $i++) {
        $contentBytes[$i] = Get-Random -Minimum 33 -Maximum 127
    }
}

# Write the changes back to the binary file
[IO.File]::WriteAllBytes($filePath, $contentBytes)
