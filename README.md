# GearVR-OSC-Handshaker
Small ofxApp that handshakes with a pre-defined list of IP's and sends them their ID's (useful for multiplayer GearVR setups using OSC).

## How to use
You can add "clients" to a list to link IP-addresses to specific Motive rigidbodies. Each client has the following information:
- IP Address
- Rigidbody Name

When the app receives a "handshake" on <b>port 5150</b> (an OSCMessage with address <b>"/gear-handshake"</b>), it will check the incoming IP with the list.

If it finds a match (first match has priority) it send back an OSCMessage on <b>port 6505</b> with address "/gear-handshake-reply" and the linked rigidbody name.

If it does not find a match, it sends back on OSCMessage on <b>port 6505</b> with address "/gear-handshake-badip".

## Virtual Positions
The Handshake ofxApp also serves as a hub for virtual positions of tracked objects. This links closely with the UnityOSCToolkit.

Once the UnityOSCToolkit's GearVR setup succesfully handshakes, and if "vertical walking" is enabled, Unity will send "virtual gear updates" (OSCMesssage with address "/gear-virtualpos", port <b>5150</b>) to this ofxApp, these messages are then forwarded to all other clients. They contain a 3D world position and the name of the client (so they can be matched on the other end).
