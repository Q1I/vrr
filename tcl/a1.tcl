# Define options
set val(chan)   Channel/WirelessChannel         ;# channel type
set val(prop)   Propagation/TwoRayGround        ;# radio-propagation model
set val(netif)  Phy/WirelessPhy                 ;# network interface type
set val(mac)    Mac/802_11                      ;# MAC type
set val(ifq)    Queue/DropTail/PriQueue         ;# interface queue type
set val(ll)     LL                              ;# link layer type
set val(ant)    Antenna/OmniAntenna             ;# antenna model
set val(ifqlen) 50                              ;# max packet in ifq
set val(seed)   0.0
#set val(rp)     AODV                            ;# routing protocol
set val(rp)     VRR                       ;# routing protocol
set val(nn)     4                              ;# number of nodes
set val(x)      400.0                           ;# X dimension of topography
set val(y)      400.0                           ;# Y dimension of topography
set val(stop)   10.0                            ;# time to stop simulation

# Create a simulator instance
set ns_ [new Simulator]

# Open the Trace file
set tracefile [open a1-out.tr w]
$ns_ trace-all $tracefile

# Open the NAM trace file
set namfile [open a1-out.nam w]
$ns_ namtrace-all-wireless $namfile $val(x) $val(y)

# Set up topography object
set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Create God
set god_ [create-god $val(nn)]

# Global node settings
$ns_ node-config -adhocRouting $val(rp) \
                 -llType $val(ll) \
                 -macType $val(mac) \
                 -ifqType $val(ifq) \
                 -ifqLen $val(ifqlen) \
                 -antType $val(ant) \
                 -propType $val(prop) \
                 -phyType $val(netif) \
                 -channelType $val(chan) \
                 -topoInstance $topo \
                 -agentTrace ON \
                 -routerTrace ON \
                 -macTrace OFF \
                 -movementTrace OFF                    
        
# Define nodes
for {set i 0} {$i < $val(nn) } {incr i} {
        set node_($i) [$ns_ node]    
        $node_($i) random-motion 0        ;# disable random motion
}

# Provide initial (X,Y,Z) co-ordinates for nodes
set k 0
for {set i 0} {$i < 5 } {incr i} {
	for {set j 0} {$j < 5 } {incr j} {
		# pos
		#$node_($k) set X_ [expr $j * 200]
		#$node_($k) set Y_ [expr $i * 200]
		#$node_($k) set Z_ 0.0
		# traffic
		
		incr k
	}
}

$node_(0) set X_ 430.0
$node_(0) set Y_ 110.0
$node_(0) set Z_ 0.0

$node_(1) set X_ 230.0
$node_(1) set Y_ 140.0
$node_(1) set Z_ 0.0

$node_(2) set X_ 240.0
$node_(2) set Y_ 80.0
$node_(2) set Z_ 0.0

$node_(3) set X_ 100.0
$node_(3) set Y_ 100.0
$node_(3) set Z_ 0.0


# Define master nodes size in nam
for {set i 0} {$i < $val(nn)} {incr i} {
        $ns_ initial_node_pos $node_($i) 50
}

# Setup traffic flow between nodes
set tcp [new Agent/TCP]
$ns_ attach-agent $node_(0) $tcp

set sink [new Agent/TCPSink]
$ns_ attach-agent $node_(2) $sink

$ns_ connect $tcp $sink

set ftp [new Application/FTP]
$ftp attach-agent $tcp

# Tell node when simulation ends
for {set i 0} {$i < $val(nn)} {incr i} {
        $ns_ at $val(stop) "$node_($i) reset";
}

# Define a 'finish' procedure
proc finish {} {
        global ns_ tracefile
        $ns_ flush-trace
        close $tracefile
        exec nam a1-out.nam &
        exit 0
}

# Schedule events
$ns_ at 0.3 "$ftp start" 
$ns_ at [expr $val(stop) - 0.000001] "finish"
$ns_ at $val(stop) "puts \"NS EXITING...\" ; $ns_ halt"

# Start the simulation
puts "Starting Simulation..."
$ns_ run
