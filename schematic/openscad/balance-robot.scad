$fn=200;

use <ESP8266Models.scad>

module plate(width) {
    color("Azure") {
        minkowski() {
            cube([150, width, 1], true);
            cylinder(r=5,h=1);
        }
    }
}

module standoff(height) {
    color("Goldenrod")
    cylinder(r=2, h=height, $fn=6);
}

module standoff_plate() {
    translate([75, 40, 0]) standoff(70);
    translate([75, -40, 0]) standoff(70);
    translate([-75, 40, 0]) standoff(70);
    translate([-75, -40, 0]) standoff(70);
}

module standoff_board() {
    translate([22, 10, 0]) standoff(10);
    translate([22, -10, 0]) standoff(10);
    translate([-22, 10, 0]) standoff(10);
    translate([-22, -10, 0]) standoff(10);
}

module bracket_pololu() {
}

module bracket_simple() {
    translate([32, -20, -2])
    color("Black") {
        minkowski() {
            cube([40, 40, 1]);
            cylinder(r=1, h=1);
        }
        translate([40, 0, 0]) rotate(90, [0, 1, 0])
        difference() {
            hull() {
                cube([20, 40, 1]);
                translate([25, 20, 0]) cylinder(r=20, h=1);
            }
            translate([26, 20, -1]) cylinder(r=8, h=3);
        }
    }
}

module bracket() {
    bracket_simple();
}

module brackets() {
    bracket();
    rotate(180, [0, 0, 1]) bracket();
}

module motor_pololu() {
    rotate(90, [0, 1, 0])
    translate([3, -18, 12])
    import("pololu-37D-encoder.STL");
}

module motor_simple() {
    rotate(90, [0, 1, 0])
    translate([18, 0, 29]) {
        color("Gainsboro") cylinder(50, 15, 15);
        translate([0, 0, 42]) color("Gray") cylinder(20, 2, 2);
    }
}

module motor() {
    color("Gainsboro")
    motor_pololu();
}

module motors() {
    motor();
    rotate(180, [0, 0, 1]) motor();
}

module wheel() {
    rotate(90, [0, 1, 0])
    translate([28, 0, 82]) {
    hull() {
        cylinder(2, 35, 36);
        translate([0, 0, 6]) cylinder(2, 36, 35);
    };
    }
}

module wheels() {
    wheel();
    rotate(180, [0, 0, 1]) wheel();
}

module battery() {
    translate([0, 0, 10]) {
        color("RoyalBlue") cube([100, 36, 20], true);
        translate([-25, -3, 10]) color("Black") text("2S Lipo");
    }
}

translate([0, 0, 0]) {
    plate(80);
    standoff_plate();
    brackets();
    motors();
    wheels();
}
translate([0, 0, 70]) {
    plate(80);
    rotate(180, [1, 0, 0]) {
        standoff_board();
        translate([0, 0, 10]) rotate(90, [0, 0, 1]) NodeMCU();
    }
    battery();
}
