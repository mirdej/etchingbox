// ============================================================================
// Double-Sided UV-LED Illuminator Case (Laser-Cut OpenSCAD Model)
// Project: SGMK EtchingBox (Aetzbox)
// Target PCB: Electronics/UV-Leds/UV-Leds.kicad_pcb (180 x 120 mm, 84 UV LEDs)
// ============================================================================
// Design Notes:
// - Glue-together finger-jointed 4mm plywood enclosure (no T-slots).
// - Both UV-LED PCBs (180 x 120 mm) screw directly flat onto the inside of the
//   Top & Bottom plywood plates via their 8 semicircular edge notches (no spacers).
// - Top Cover includes calibrated cutouts for the top PCB's backside 0.96" OLED
//   screen and 4 tactile pushbuttons (SW1, SW2, SW3, SW4).
// - Uses existing 2x clear plexi carrier plates (180 x 120 x 4.8 mm, R=4mm corners),
//   sliding in from the SHORTER side (-X side wall, 120mm wide).
// - 2-PIECE SHORT ENTRY WALL (-X side): Split into two separate laser-cut pieces
//   (Lower Wall: 122 x 33mm, Upper Wall: 122 x 31.8mm). Completely eliminates the
//   fragile 0.4mm side bridges from the previous 1-piece design!
//   - Lower piece top edge is perfectly flush with the rail runner (Z = -5.6 mm).
//   - Upper piece bottom edge sits at slot ceiling (Z = +6.8 mm).
//   - Smooth central finger pinch notches for easy removal of the plexi carrier plates.
// - ROBUST FLAT SIDE RAILS WITH 3 SLENDER-NECK FLEXIBLE SPRINGS:
//   Each long wall has a single flat 4mm laser-cut rail piece (187.6 x 48 mm)
//   screwed directly flat against the inside of the wall.
//   Beam mechanics for 4mm birch plywood:
//   - Root neck thickness: 1.8 mm with a rounded hairpin U-loop relief cut (R_in=2.2mm)
//     giving a gentle bending stress of ~20-37 MPa for 1.3-2.4mm deflection (safe under fatigue).
//   - Generous 6.0 mm clearance gap at the tip to eliminate rubbing or jamming against the wall.
//   - Clamping force: ~1.4-2.6 N (~140-260g) per spring finger (~0.85-1.6 kg total force across
//     the 6 spring fingers on both rails).
//   - Main arm expands to 4.5 mm thick for a rigid paddle.
//   - Smooth ski-ramp rounded entry tip at +3.3 mm (resting gap 8.9 mm).
// ============================================================================

/* [View & Export Mode] */
// Select "3d_assembly" to inspect the 3D box, "3d_exploded" for exploded view, or "2d_cut_sheet" to export DXF/SVG for laser cutting.
render_mode = "3d_assembly"; // [3d_assembly:3D Assembled View, 3d_exploded:3D Exploded View, 2d_cut_sheet:2D Laser Cut Sheet (4mm Plywood Case)]

// Show the UV-LED PCBs, OLED + buttons, and the 180x120x4.8mm plexi sandwich in 3D mode
show_internals = true;
// Slide the plexi sandwich out through the short-side (-X) slot in 3D view (0 = fully inserted & centered, 40 = partially pulled out)
drawer_pullout = 75; // [0:1:160]

/* [Material & Laser Parameters] */
// Thickness of the laser-cut plywood sheet for the enclosure (mm) - adjustable with 0.1mm precision!
thickness = 4.6; // [1.0:0.1:10.0]
// Laser kerf compensation (mm) - expands tabs slightly for tight glue fit (0 = nominal)
kerf = 0.10; // [0.0:0.01:0.30]
// Target width of finger-joint tabs along the edges (mm) - safely adjustable to any value!
tab_width = 30.0; // [6.0:0.5:60.0]
// Corner rounding radius on outer plates (mm)
corner_r = 0.0; // [0.0:0.5:10.0]

/* [UV-LED PCB Specifications (from UV-Leds.kicad_pcb)] */
pcb_w = 180.0;
pcb_d = 120.0;
pcb_t = 1.6;
// Notch offsets from PCB center (long edges: X = +/-62mm; short edges: Y = +/-34mm)
pcb_notch_x = 62.0;
pcb_notch_y = 34.0;
pcb_notch_r = 3.0; // 6mm diameter semicircle notch, center inset 3mm from edge

/* [Top Cover OLED & 4-Button Cutouts (Backside of Top PCB)] */
// Orientation of the Top PCB's OLED/Button end: -1 = towards the Short-Side Entry Slot (-X), +1 = towards the Closed Back End (+X)
ui_end_sign = -1; // [-1:Entry Slot Side (-X), 1:Closed Rear Side (+X)]
// Distance from the short edge of the 180mm PCB to the center of the 0.96" OLED module (mm)
oled_dist_from_edge = 19.25;
// Width of OLED cutout across the 120mm PCB axis (mm)
oled_cut_y = 29.0;
// Length of OLED cutout along the 180mm PCB axis (mm)
oled_cut_x = 28.5;
// Distance from the short edge of the 180mm PCB to the outer & inner button rows (SW3/SW4 and SW2/SW1) (mm)
btn_row1_from_edge = 16.5;
btn_row2_from_edge = 28.0;
// Lateral offset of the 4 buttons from the PCB centerline across the 120mm width (mm)
btn_offset_y = 24.5;
// Width (across 120mm axis) and length (along 180mm axis) of each rectangular button opening (mm)
btn_cut_y = 12.0;
btn_cut_x = 9.0;

/* [Front Entry Wall Finger Grip Opening] */
// Width of the central finger grip opening on the front entry wall (mm)
finger_hole_w = 70.0; // [40.0:1.0:90.0]
// Depth of the finger notch into the Lower Entry Wall below the slot runner (mm)
finger_hole_lower_depth = 16.0; // [5.0:0.5:22.0]
// Height of the finger notch into the Upper Entry Wall above the slot ceiling (mm)
finger_hole_upper_depth = 16.0; // [5.0:0.5:20.0]
// Corner rounding radius on the finger notch (mm)
finger_hole_r = 12.0; // [4.0:0.5:20.0]

/* [Existing Clear Plexi Carrier Plates] */
plexi_w = 180.0;
plexi_d = 120.0;
// Thickness of the clear plexiglass carrier plates (mm)
plexi_t = 4.8; // [2.0:0.1:8.0]
plexi_corner_r = 4.0;
// Thickness of target double-sided copper PCB shown in 3D view (0.5mm to 1.6mm)
target_pcb_t = 1.6; // [0.5:0.1:1.6]
min_target_pcb_t = 0.5;
max_target_pcb_t = 1.6;

/* [Enclosure & Optical Spacing] */
// Side clearance along Y (between the 120mm plexi/PCB and the Long Front/Back walls) (mm per side)
// With 4mm thick flat side rails mounted inside the long walls, each rail overlaps the 120mm plexi by (4.0 - side_clearance_y) = 3.0mm!
side_clearance_y = 1.0;
// End clearance along X (around the 180mm PCB/plexi to the Short Left/Right walls) (mm per side)
end_clearance_x = 4.0;
// Distance between the two facing UV-LED PCBs (mm) - ~74mm matches original Insoleuse (~35mm per side to target PCB)
pcb_facing_dist = 74.0;
// Pilot/screw hole diameter on Top & Bottom covers for screwing the 8 PCB notches directly to the plywood (mm)
pcb_screw_d = 2.5; // [1.5:0.1:3.5]
// Pilot hole diameter on the outer side walls (long walls) for wood screws (mm) - smaller for a tight bite!
side_wall_screw_d = 2.0; // [1.0:0.1:3.5]
// Screw hole diameter on the inner side rails (mm)
rail_screw_d = 2.0; // [1.0:0.1:3.5]
mount_hole_d = pcb_screw_d; // legacy alias

/* [Spring-Loaded Flat Side Rails (Screwed Inside Long Walls)] */
// Total height of the single-piece flat side rail (mm)
spring_rail_h = 48.0; // [40:1:60]
// Slender flexible neck thickness at the hinge root (mm) - calculated for ~20-37 MPa stress in birch plywood
spring_neck_t = 1.8; // [1.2:0.1:3.0]
// Rigid paddle arm thickness (mm)
spring_arm_h = 4.5; // [3.5:0.1:6.0]
// Resting gap between runner surface and lowest tip of spring finger (mm)
spring_resting_gap = 8.9;

// ============================================================================
// Derived Dimensions
// ============================================================================
$fn = 48;

inner_w = pcb_w + 2 * end_clearance_x;  // 180 + 8 = 188.0 mm
inner_d = pcb_d + 2 * side_clearance_y; // 120 + 2 = 122.0 mm
// PCBs are screwed directly onto the inner surfaces of the Top & Bottom plates:
inner_h = pcb_facing_dist + 2 * pcb_t;  // 74 + 3.2 = 77.2 mm

outer_w = inner_w + 2 * thickness;      // 196.0 mm (with 4mm plywood)
outer_d = inner_d + 2 * thickness;      // 130.0 mm (with 4mm plywood)
outer_h = inner_h + 2 * thickness;      // 85.2 mm (with 4mm plywood)

// Sandwich & Entry Slot dimensions (sliding in from the -X short side)
max_sandwich_t = 2 * plexi_t + max_target_pcb_t; // 2*4.8 + 1.6 = 11.2 mm
min_sandwich_t = 2 * plexi_t + min_target_pcb_t; // 2*4.8 + 0.5 = 10.1 mm
cur_sandwich_t = 2 * plexi_t + target_pcb_t;

// Bottom runner top surface sits at Z = -max_sandwich_t/2 (-5.6 mm)
rail_runner_top_z = -max_sandwich_t / 2; // -5.6 mm
// Slide channel ceiling in the rail (7.5 mm) - higher than max sandwich (+5.6 mm) so only springs touch!
rail_channel_ceil_z = 7.5;

// Entry slot in the short -X wall goes from rail_runner_top_z (-5.6mm) to +max_sandwich_t/2 + 1.2mm (+6.8mm)
drawer_slot_h = max_sandwich_t + 1.2;    // 12.4 mm
drawer_slot_center_z = 0.6;
drawer_slot_w = plexi_d + 1.2;           // 121.2 mm wide slot on the short (-X) wall

// Safe vertical zone on the short walls above and below the central drawer slot
short_wall_half_span = (inner_h - drawer_slot_h) / 2 - 4.0;
short_wall_half_center = drawer_slot_h/2 + 2.0 + short_wall_half_span/2;

// Screw positions for mounting the flat side rails onto the Long Front/Back walls
rail_screw_pts = [
    [-74.0, -14.5],
    [  0.0, -14.5],
    [ 74.0, -14.5],
    [-84.0,  19.0],
    [  0.0,  19.0],
    [ 80.0,  19.0]
];

explode_dist = (render_mode == "3d_exploded") ? 28.0 : 0.0;

// 8 mounting hole coordinates relative to PCB center (from UV-Leds.kicad_pcb Edge.Cuts)
pcb_mount_pts = [
    [-pcb_notch_x, -(pcb_d/2 - 3)], // Top-Left  (-62, -57)
    [ pcb_notch_x, -(pcb_d/2 - 3)], // Top-Right ( 62, -57)
    [-pcb_notch_x,  (pcb_d/2 - 3)], // Bot-Left  (-62,  57)
    [ pcb_notch_x,  (pcb_d/2 - 3)], // Bot-Right ( 62,  57)
    [-(pcb_w/2 - 3), -pcb_notch_y], // Left-Top  (-87, -34)
    [-(pcb_w/2 - 3),  pcb_notch_y], // Left-Bot  (-87,  34)
    [ (pcb_w/2 - 3), -pcb_notch_y], // Right-Top ( 87, -34)
    [ (pcb_w/2 - 3),  pcb_notch_y]  // Right-Bot ( 87,  34)
];

// Top PCB OLED & 4 Button center positions relative to PCB center
oled_center_x = ui_end_sign * (pcb_w/2 - oled_dist_from_edge);
btn_row1_x = ui_end_sign * (pcb_w/2 - btn_row1_from_edge);
btn_row2_x = ui_end_sign * (pcb_w/2 - btn_row2_from_edge);
top_button_pts = [
    [btn_row1_x, -btn_offset_y], // SW3 (or SW4)
    [btn_row1_x,  btn_offset_y], // SW4 (or SW3)
    [btn_row2_x, -btn_offset_y], // SW2 (or SW1)
    [btn_row2_x,  btn_offset_y]  // SW1 (or SW2)
];

// LED coordinates relative to PCB center (extracted from UV-Leds.kicad_pcb)
led_coords = [
    [-86.0, 48.0], [43.0, -48.23], [-42.5, 34.05], [31.0, -56.0], [-86.0, 18.0],
    [-71.0, -48.23], [62.0, 17.59], [24.0, 50.5], [33.5, 1.14], [62.0, 50.5],
    [24.0, -48.23], [-33.0, 50.5], [78.0, -56.0], [-52.0, -15.32], [-28.0, -56.0],
    [-80.5, 1.14], [-71.0, 17.59], [-1.5, 56.25], [5.0, 50.5], [5.0, -48.23],
    [33.5, -31.77], [-80.5, -31.77], [-71.0, -15.32], [5.0, 17.59], [-33.0, -15.32],
    [71.5, -31.77], [81.0, 50.5], [-14.0, -15.32], [14.5, -31.77], [24.0, 17.59],
    [86.16, -39.5], [-14.0, 50.5], [-85.69, -15.5], [52.5, 1.14], [43.0, -15.32],
    [33.5, 34.05], [-23.5, -31.77], [-4.5, 34.05], [52.37, -56.0], [86.0, 43.0],
    [33.0, 56.0], [-42.5, -31.77], [-79.25, 34.05], [-52.37, -56.0], [-85.0, -43.0],
    [-4.5, -31.77], [-23.5, 1.14], [52.37, 56.0], [52.5, -31.77], [-28.0, 56.0],
    [-14.0, -48.23], [-78.0, -56.0], [-61.5, 1.14], [71.5, 34.05], [71.5, 1.14],
    [-71.0, 50.5], [5.0, -15.32], [43.0, 17.59], [43.0, 50.5], [-33.0, -48.23],
    [24.0, -15.32], [14.5, 1.14], [-52.0, 17.59], [72.16, 56.0], [62.0, -15.32],
    [-42.5, 1.14], [-61.5, 34.05], [81.0, -15.32], [87.0, -3.5], [-4.5, 1.14],
    [-23.5, 34.05], [-33.0, 17.59], [-61.5, -31.77], [-52.0, -48.23], [14.5, 34.05],
    [62.0, -48.23], [81.0, -48.23], [-0.56, -56.5], [-78.0, 56.0], [-52.0, 50.5],
    [-55.34, 56.0], [52.5, 34.05], [81.0, 17.59], [-14.0, 17.59]
];

// ============================================================================
// Dynamic Finger-Joint Helper Modules (Safe for ANY `tab_width`)
// ============================================================================

function auto_tab_count(span_L, target_w, margin=8.0) =
    max(1, floor(((span_L - 2 * margin) / max(3.0, target_w) + 1) / 2));

function auto_tab_w(span_L, target_w, margin=8.0) =
    min(max(3.0, target_w), max(4.0, span_L - 2 * margin));

module finger_pattern_auto(span_L, target_w, depth, is_slot=false, margin=8.0) {
    n_tabs = auto_tab_count(span_L, target_w, margin);
    w = auto_tab_w(span_L, target_w, margin);
    k = is_slot ? -kerf : kerf;
    for (i = [0 : n_tabs - 1]) {
        translate([(i - (n_tabs - 1)/2) * 2 * w, 0])
            square([w + k, depth + 0.02], center=true);
    }
}

// ============================================================================
// 2D Laser-Cut Panel Profiles (4mm Plywood)
// ============================================================================

// 1. TOP & BOTTOM PLATES (196 x 130 mm outer footprint)
module panel_top_bottom_2d(is_top=false) {
    offset(r = kerf/2)
    difference() {
        offset(r = corner_r) offset(delta = -corner_r)
            square([outer_w, outer_d], center=true);

        for (sy = [-1, 1]) {
            translate([0, sy * (inner_d/2 + thickness/2)])
                finger_pattern_auto(inner_w, tab_width, thickness, is_slot=true, margin=10.0);
        }

        for (sx = [-1, 1]) {
            translate([sx * (inner_w/2 + thickness/2), 0])
            rotate([0, 0, 90])
                finger_pattern_auto(inner_d, tab_width, thickness, is_slot=true, margin=10.0);
        }

        for (pt = pcb_mount_pts) {
            translate(pt) circle(d = pcb_screw_d);
        }

        if (is_top) {
            translate([oled_center_x, 0])
                offset(r = 1.0) offset(delta = -1.0)
                square([oled_cut_x, oled_cut_y], center=true);

            for (bpt = top_button_pts) {
                translate(bpt)
                    offset(r = 1.0) offset(delta = -1.0)
                    square([btn_cut_x, btn_cut_y], center=true);
            }
        }
    }
}

// 2. LONG FRONT (-Y) & BACK (+Y) WALLS (196 x 85.2 mm)
module panel_long_wall_2d() {
    offset(r = kerf/2)
    difference() {
        union() {
            square([outer_w, inner_h], center=true);

            for (sy = [-1, 1]) {
                translate([0, sy * (inner_h/2 + thickness/2)])
                    finger_pattern_auto(inner_w, tab_width, thickness, is_slot=false, margin=10.0);
            }
        }

        for (sx = [-1, 1], sz = [-1, 1]) {
            translate([sx * (inner_w/2 + thickness/2), sz * short_wall_half_center])
            rotate([0, 0, 90])
                finger_pattern_auto(short_wall_half_span, tab_width, thickness, is_slot=true, margin=2.0);
        }

        for (spt = rail_screw_pts) {
            translate(spt) circle(d = side_wall_screw_d);
        }
    }
}

// 3a. SHORT ENTRY LOWER WALL (-X Side, Lower Half: 122 x 33 mm)
// Top edge is flush with rail runner (Z = -5.6 mm) so plexi slides right onto the rail!
module panel_short_entry_lower_2d() {
    h = rail_runner_top_z - (-inner_h/2); // 33.0 mm
    tab_c_local = -0.3; // matches lower mortise slots in long walls (Z = -22.4 mm)

    offset(r = kerf/2)
    difference() {
        union() {
            // Main rectangular body
            square([inner_d, h], center=true);

            // Bottom tabs into Bottom Cover
            translate([0, -(h/2 + thickness/2)])
                finger_pattern_auto(inner_d, tab_width, thickness, is_slot=false, margin=10.0);

            // Side vertical tabs into Long Front & Back Walls
            for (sx = [-1, 1]) {
                translate([sx * (inner_d/2 + thickness/2), tab_c_local])
                rotate([0, 0, 90])
                    finger_pattern_auto(short_wall_half_span, tab_width, thickness, is_slot=false, margin=2.0);
            }
        }

        // Large & wide finger grip opening dipping down from top edge (h/2)
        hw = finger_hole_w / 2 - finger_hole_r;
        hull() {
            translate([-hw, h/2 - finger_hole_lower_depth + finger_hole_r]) circle(r = finger_hole_r);
            translate([ hw, h/2 - finger_hole_lower_depth + finger_hole_r]) circle(r = finger_hole_r);
            translate([-finger_hole_w/2, h/2]) square([finger_hole_w, 20]);
        }
    }
}

// 3b. SHORT ENTRY UPPER WALL (-X Side, Upper Half: 122 x 31.8 mm)
// Bottom edge sits at slot ceiling (Z = +6.8 mm) giving a clear 12.4 mm slide opening
module panel_short_entry_upper_2d() {
    h = inner_h/2 - (rail_runner_top_z + drawer_slot_h); // 31.8 mm
    tab_c_local = -0.3; // matches upper mortise slots in long walls (Z = +22.4 mm)

    offset(r = kerf/2)
    difference() {
        union() {
            // Main rectangular body
            square([inner_d, h], center=true);

            // Top tabs into Top Cover
            translate([0, h/2 + thickness/2])
                finger_pattern_auto(inner_d, tab_width, thickness, is_slot=false, margin=10.0);

            // Side vertical tabs into Long Front & Back Walls
            for (sx = [-1, 1]) {
                translate([sx * (inner_d/2 + thickness/2), tab_c_local])
                rotate([0, 0, 90])
                    finger_pattern_auto(short_wall_half_span, tab_width, thickness, is_slot=false, margin=2.0);
            }
        }

        // Large & wide finger grip opening rising up from bottom edge (-h/2)
        hw = finger_hole_w / 2 - finger_hole_r;
        hull() {
            translate([-hw, -h/2 + finger_hole_upper_depth - finger_hole_r]) circle(r = finger_hole_r);
            translate([ hw, -h/2 + finger_hole_upper_depth - finger_hole_r]) circle(r = finger_hole_r);
            translate([-finger_hole_w/2, -h/2 - 20]) square([finger_hole_w, 20]);
        }
    }
}

// 4. SHORT CLOSED/BACK WALL (+X Side, 130 x 85.2 mm)
module panel_short_closed_2d() {
    offset(r = kerf/2)
    difference() {
        union() {
            square([inner_d, inner_h], center=true);

            for (sy = [-1, 1]) {
                translate([0, sy * (inner_h/2 + thickness/2)])
                    finger_pattern_auto(inner_d, tab_width, thickness, is_slot=false, margin=10.0);
            }

            for (sx = [-1, 1], sz = [-1, 1]) {
                translate([sx * (inner_d/2 + thickness/2), sz * short_wall_half_center])
                rotate([0, 0, 90])
                    finger_pattern_auto(short_wall_half_span, tab_width, thickness, is_slot=false, margin=2.0);
            }
        }

        translate([-42, inner_h/2 - 14]) circle(d = 10.0);
    }
}

// 5. ROBUST SINGLE-PIECE FLAT SPRING RAIL (2x pieces: 187.6 x 48.0 x 4.0 mm)
// - Slender 1.8mm flexible hinge neck with rounded hairpin U-loop relief cut (R_in=2.2mm, R_out=4.0mm)
// - SHORTENED SPRING ARM with a generous 6.0mm clearance gap at the tip to prevent
//   any rubbing or jamming against the cavity wall when flexing upwards!
// - Smooth ski-ramp rounded entry tip at +3.3mm (resting gap 8.9mm).
// - Flexure Mechanics for 4mm Laser-Cut Birch Plywood:
//   - Modulus of Elasticity E ≈ 8000 MPa, sheet thickness b = 4.0 mm.
//   - Lever arm L ≈ 31.0 mm, active hinge arc length L_arc ≈ 15.0 mm.
//   - Deflection delta = 1.3 mm (0.5mm PCB) to 2.4 mm (1.6mm PCB).
//   - Max bending stress: sigma = (E * t * delta) / (2 * L_arc * L_arm) ≈ 20 to 37 MPa (safe under fatigue).
//   - Soft continuous clamping force: F = ~1.4 N (~140g) to ~2.6 N (~260g) per finger.
//     Total clamping force across all 6 fingers = ~0.85 to 1.6 kg (positive retention without jamming).

function spring_arm_pts_2d(u_x, neck_x, tip_x, wall_x, t_neck=1.8) = concat(
    // 1. Underside curve dipping gently from ceiling (+7.5mm) down to rest (+3.3mm)
    [
        [u_x + 1.0, 7.5],
        [neck_x - 1.5, 7.6],
        [neck_x, 7.8]
    ],
    [for (i = [1 : 7])
        let (t = i / 7.0)
        [neck_x + (tip_x - neck_x)*t, 7.8 - 4.5*sin(t * 90)]
    ],
    // 2. Shortened ski-ramp tip with smooth rounded nose (ends at tip_x + 3.0, leaving 6mm gap to wall_x)
    [
        [tip_x + 1.5, 4.1],
        [tip_x + 3.0, 5.5],
        [tip_x + 2.5, 7.5],
        [tip_x + 0.5, 7.8]
    ],
    // 3. Top surface of the rigid paddle arm back toward neck
    [for (i = [5 : -1 : 0])
        let (t = i / 5.0, 
             xi = (neck_x + 4.0) + ((tip_x + 0.5) - (neck_x + 4.0))*t,
             yi = (7.8 + t_neck) + (7.8 - (7.8 + t_neck))*sin(t * 90))
        [xi, yi]
    ],
    // 4. Slender flexible hinge neck transition (t_neck = 1.8mm thick)
    [
        [neck_x + 2.0, 7.8 + t_neck + 0.2],
        [neck_x, 7.8 + t_neck]
    ],
    // 5. Rounded hairpin U-loop relief cut curving 180 degrees backwards around the corner
    [for (i = [0 : 6])
        let (a = -90 + i * (180/6.0),
             u_cy = 7.8 + t_neck + 2.2, // 11.8 mm
             u_ry = 2.2,
             u_rx = (neck_x - u_x) * 0.8)
        [u_x + u_rx * (1.0 - cos(a)), u_cy + u_ry * sin(a)]
    ],
    // 6. Relief slot top roof extending horizontally to wall_x, then dropping vertically to ceiling
    [
        [wall_x, 14.8],
        [wall_x, 7.5]
    ]
);

function reverse_list(l) = [for (i = [len(l)-1 : -1 : 0]) l[i]];

module panel_spring_rail_2d() {
    rail_w = inner_w - 0.4; // 187.6 mm
    h = spring_rail_h;      // 48.0 mm
    
    // 3 Shortened spring fingers with 6.0mm tip clearance gap:
    sp1_rev = reverse_list(spring_arm_pts_2d(-76.0, -70.0, -42.0, -33.0, spring_neck_t));
    sp2_rev = reverse_list(spring_arm_pts_2d(-24.0, -18.0,  10.0,  19.0, spring_neck_t));
    sp3_rev = reverse_list(spring_arm_pts_2d( 28.0,  34.0,  62.0,  71.0, spring_neck_t));

    full_pts = concat(
        [
            [-rail_w/2, h/2],
            [ rail_w/2, h/2],
            [ rail_w/2, -h/2],
            [-rail_w/2, -h/2],
            [-rail_w/2, -7.6],
            [-rail_w/2 + 8.0, -5.6],
            [ 90.0, -5.6],
            [ 90.0,  7.5],
            [ 71.0,  7.5]
        ],
        sp3_rev,
        [[19.0, 7.5]],
        sp2_rev,
        [[-33.0, 7.5]],
        sp1_rev,
        [
            [-rail_w/2 + 8.0, 7.5],
            [-rail_w/2, 9.5]
        ]
    );

    offset(r = kerf/2)
    difference() {
        polygon(full_pts);

        // 6 Screw holes matching the Long Front & Back walls
        for (spt = rail_screw_pts) {
            translate(spt) circle(d = rail_screw_d);
        }
    }
}

// ============================================================================
// 3D Models of the UV-LED PCB, Backside OLED/Buttons & Existing Plexi Sandwich
// ============================================================================

module uv_led_pcb_3d(has_backside_ui=false) {
    color(has_backside_ui ? [0.92, 0.92, 0.94] : [0.12, 0.14, 0.18])
    linear_extrude(height = pcb_t, center = true)
    difference() {
        offset(r = 2.5) offset(delta = -2.5)
            square([pcb_w, pcb_d], center = true);
        for (pt = pcb_mount_pts) {
            translate(pt) circle(r = pcb_notch_r);
            if (abs(pt[1]) > 50)
                translate([pt[0], pt[1] + sign(pt[1])*1.5])
                    square([pcb_notch_r*2, 3.1], center=true);
            else
                translate([pt[0] + sign(pt[0])*1.5, pt[1]])
                    square([3.1, pcb_notch_r*2], center=true);
        }
    }

    for (pt = led_coords) {
        translate([pt[0], pt[1], pcb_t/2 + 0.4]) {
            color([0.92, 0.92, 0.96])
                cube([3.5, 2.8, 0.8], center = true);
            color([0.65, 0.35, 0.95])
                translate([0, 0, 0.42])
                cylinder(d = 2.2, h = 0.1, center = true);
        }
    }

    if (has_backside_ui) {
        translate([oled_center_x, 0, -pcb_t/2 - 2.0]) {
            color([0.10, 0.12, 0.25])
                cube([27.5, 27.8, 3.8], center = true);
            color([0.05, 0.05, 0.05])
                translate([0, 0, -1.95])
                cube([19.0, 26.0, 0.8], center = true);
        }
        for (bpt = top_button_pts) {
            translate([bpt[0], bpt[1], -pcb_t/2 - 1.2]) {
                color([0.82, 0.82, 0.85])
                    cube([5.2, 6.2, 2.4], center = true);
                color([0.55, 0.22, 0.18])
                    translate([0, 0, -1.6])
                    cube([2.4, 3.2, 1.4], center = true);
            }
        }
    }
}

module existing_plexi_plate_3d() {
    color([0.75, 0.92, 1.0, 0.48])
    linear_extrude(height = plexi_t, center = true)
        offset(r = plexi_corner_r) offset(delta = -plexi_corner_r)
        square([plexi_w, plexi_d], center = true);
}

module drawer_sandwich_3d() {
    z_bot_plexi = rail_runner_top_z + plexi_t/2;
    z_pcb       = rail_runner_top_z + plexi_t + target_pcb_t/2;
    z_top_plexi = rail_runner_top_z + plexi_t + target_pcb_t + plexi_t/2;

    translate([-drawer_pullout - explode_dist * 2.2, 0, 0]) {
        translate([0, 0, z_bot_plexi - explode_dist * 0.25])
            existing_plexi_plate_3d();

        color([0.78, 0.48, 0.22, 0.9])
        translate([0, 0, z_pcb])
            cube([160.0, 100.0, target_pcb_t], center = true);

        translate([0, 0, z_top_plexi + explode_dist * 0.25])
            existing_plexi_plate_3d();
    }
}

// ============================================================================
// 3D Assembly & Exploded View
// ============================================================================

module assembly_3d() {
    // 1. Bottom Plywood Plate
    color([0.76, 0.60, 0.42, 0.92])
    translate([0, 0, -inner_h/2 - thickness/2 - explode_dist * 1.4])
        linear_extrude(height = thickness, center = true)
        panel_top_bottom_2d(false);

    // 2. Top Plywood Cover (with OLED & 4-button cutouts)
    color([0.76, 0.60, 0.42, 0.65])
    translate([0, 0, inner_h/2 + thickness/2 + explode_dist * 1.4])
        linear_extrude(height = thickness, center = true)
        panel_top_bottom_2d(true);

    // 3. Long Front (-Y) & Back (+Y) Walls
    for (sy = [-1, 1]) {
        color([0.70, 0.54, 0.36, 0.85])
        translate([0, sy * (inner_d/2 + thickness/2 + explode_dist), 0])
        rotate([90, 0, 0])
            linear_extrude(height = thickness, center = true)
            panel_long_wall_2d();
    }

    // 4. Short Entry Wall (at -X) - 2 separate pieces (Lower & Upper)
    z_entry_lower = (-inner_h/2 + rail_runner_top_z) / 2; // -22.1 mm
    z_entry_upper = (inner_h/2 + (rail_runner_top_z + drawer_slot_h)) / 2; // +22.7 mm

    color([0.66, 0.50, 0.32, 0.88])
    translate([-(inner_w/2 + thickness/2 + explode_dist), 0, z_entry_lower])
    rotate([90, 0, -90])
        linear_extrude(height = thickness, center = true)
        panel_short_entry_lower_2d();

    color([0.66, 0.50, 0.32, 0.88])
    translate([-(inner_w/2 + thickness/2 + explode_dist), 0, z_entry_upper])
    rotate([90, 0, -90])
        linear_extrude(height = thickness, center = true)
        panel_short_entry_upper_2d();

    // 5. Short Closed Wall (at +X)
    color([0.66, 0.50, 0.32, 0.88])
    translate([(inner_w/2 + thickness/2 + explode_dist), 0, 0])
    rotate([90, 0, 90])
        linear_extrude(height = thickness, center = true)
        panel_short_closed_2d();

    // 6. 2x Robust Flat Spring Rails with Slender-Neck Hinges
    for (sy = [-1, 1]) {
        color([0.58, 0.40, 0.22, 0.95])
        translate([0, sy * (inner_d/2 - thickness/2 + explode_dist * 0.45), 0])
        rotate([90, 0, 0])
            linear_extrude(height = thickness, center = true)
            panel_spring_rail_2d();
    }

    // 7. Internal PCBs & Plexi Sandwich
    if (show_internals) {
        translate([0, 0, -inner_h/2 + pcb_t/2 - explode_dist * 0.6])
            uv_led_pcb_3d(false);

        translate([0, 0, inner_h/2 - pcb_t/2 + explode_dist * 0.6])
        rotate([180, 0, 0])
            uv_led_pcb_3d(true);

        drawer_sandwich_3d();
    }
}

// ============================================================================
// 2D Laser Cut Flat Layout (for DXF / SVG Export)
// ============================================================================

module cut_sheet_enclosure_2d() {
    gap = 10.0;

    // Row 1: Bottom Plate (left) & Top Cover with OLED/4-Button cutouts (right)
    translate([0, 0])
        panel_top_bottom_2d(false);
    translate([outer_w + gap, 0])
        panel_top_bottom_2d(true);

    // Row 2: Long Front Wall & Long Back Wall (196 x 85.2 mm each)
    row2_y = outer_d/2 + inner_h/2 + thickness + gap;
    translate([0, row2_y])
        panel_long_wall_2d();
    translate([outer_w + gap, row2_y])
        panel_long_wall_2d();

    // Row 3: Short Entry Walls (Lower & Upper) & Short Closed Wall (+X)
    row3_y = row2_y + inner_h + 2 * thickness + gap;
    translate([-(outer_w - outer_d)/2, row3_y - 20])
        panel_short_entry_lower_2d();
    translate([-(outer_w - outer_d)/2, row3_y + 24])
        panel_short_entry_upper_2d();
    translate([outer_d + gap - (outer_w - outer_d)/2, row3_y])
        panel_short_closed_2d();
    translate([outer_d + gap - (outer_w - outer_d)/2, row3_y])
        panel_short_closed_2d();

    // Row 4: 2x Robust Flat Spring Rails with Slender-Neck Hinges (187.6 x 48.0 mm each)
    row4_y = row3_y + inner_h/2 + thickness + gap + spring_rail_h/2;
    translate([0, row4_y])
        panel_spring_rail_2d();
    translate([outer_w + gap, row4_y])
        panel_spring_rail_2d();
}

// ============================================================================
// Main Entry Point
// ============================================================================

if (render_mode == "3d_assembly" || render_mode == "3d_exploded") {
    assembly_3d();
} else if (render_mode == "2d_cut_sheet") {
    cut_sheet_enclosure_2d();
}
