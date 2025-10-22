<?php

$e24 = [ 1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1 ];
//$e24 = [ 1.0, 1.2, 1.5, 2.0, 2.2, 2.7, 3.0, 3.3, 3.9, 4.7, 5.1, 6.2, 6.8, 7.5, 8.2, 9.1 ];
/*
$e24 = [
    1.00, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40,
    1.47, 1.54, 1.62, 1.69, 1.78, 1.87, 1.96, 2.05,
    2.15, 2.26, 2.37, 2.49, 2.61, 2.74, 2.87, 3.01,
    3.16, 3.32, 3.48, 3.65, 3.83, 4.02, 4.22, 4.42,
    4.64, 4.87, 5.11, 5.36, 5.62, 5.90, 6.19, 6.49,
    6.81, 7.15, 7.50, 7.87, 8.25, 8.66, 9.09, 9.53
];
*/
$leds = array(
    'red' => 1.9,
    'blue' => 3.1,
    'white' => 3.2,
    'yellow' => 2.1,
    'green' => 2.2
);

function find_closest( $name = 'blue', $voltage = 3.3, $voltageDrop = 3.0, $ledCurrent = 1.5 ) {
    global $e24;

    $closest = 10000;
    $value = 0;

    for ( $mult = 0; $mult < 6; $mult++ ) {
        $multiplier = pow( 10, $mult );

        foreach( $e24 as $oneValue ) {
            $toTest = $oneValue * $multiplier;
            $volt = $voltage - $voltageDrop;

            $current = $volt*1000/$toTest;

            if ( $current >= $ledCurrent && $current < $closest ) {
                $value = $toTest;
                $closet = $current;
            }
        }
    }

    return $value;
}

function nice_resistor( $r ) {
    if ( $r < 1000 ) {
        return $r;
    } else if ( $r >= 1000 ) {
        return ($r / 1000.0 ) . 'k';
    }
}

$voltages = array( 3.3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 );

if ( $argc != 2 ) {
    echo "Usage: php led <current_in_ma>\n";
} else {
    $current = $argv[ 1 ];

    echo "\nResistor Values For A Current of [" . $current . "ma]\n";

    echo "\n\t";
    foreach( $leds as $name => $drop ) {
        echo sprintf( "%20s", $name ) . "";
    }
    echo "\n";

    foreach( $voltages as $voltage ) {
        echo $voltage . "\t";
        foreach( $leds as $name => $drop ) {
            $value = find_closest( $name, $voltage, $drop, $current );
            echo sprintf( "%20s", sprintf( "%s (%0.2f) ma", nice_resistor( $value ), ($voltage - $drop)*1000/$value ) ) ;
        }
        echo "\n";
    }
}



