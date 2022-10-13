# USER DEFINED
$outDir = "C:\Games\Modding\Skyrim SE\MO2 - Mods\SprintTweaks\Interface\Translations"

$strings = @('czech', 'english', 'french', 'german', 'italian', 'japanese', 'polish', 'russian', 'spanish')

ForEach ($string in $strings)
{
    Copy-Item "SprintTweaks_english.txt" -Destination "$outDir\SprintTweaks_$string.txt"
}