function get_export_symbols($dll) {
	$out = .\link -dump /exports $dll
	$ret = @()
	$i = 19;
	while ($true) {
		if ($out[$i].length -eq 0) {
			break
		}
		$temp = $out[$i].trim(' ') -split ' '
		$ret += $temp[-1]
		$i++
	}
	return $ret
}

function make_def($libname, $txt, $incXTrap, $output) {
	$def = @(
		"LIBRARY	`"$libname`"",
		"",
		"EXPORTS"
	)

	$i = 1
	foreach ($name in $txt) {
		if (!$incXTrap -and $name.contains("XTrap")) {
			continue
		}
		$str = "    {0} @{1} NONAME" -f $name, $i
		$def += $str
		$i++
	}

	$def | set-content -encoding ascii $output
}

$exports = get_export_symbols "E:\LastChaosRoot\LastChaosLocalClient\LastChaosGER\Bin\Bin\Engine.dll"
make_def "Engine" $exports $false "Engine.def"
make_def "Engine" $exports $true  "Engine_XTrap.def"
