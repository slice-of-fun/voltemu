# User Handbook - Server hosting

This guide explains how to set up a public/private self hosted Volt server/lobby.

## Using a Kamatera VPS and Docker on Ubuntu

- Firstly, head over to kamatera.com and create an account. Sign in and create a new server under "My cloud", then create a new server.

- Region: Choose a location that balances latency for both you and other players (example: New York for US-Europe connections if the host is based in the US).

- Next, under Server OS Images, select Ubuntu 24.04 LTS. Configure CPU/RAM/specs as desired for your server. Complete the creation process.

- Enable the Kamatera firewall and set default policy: IN: DROP, OUT: ACCEPT.
 - After setting the default policy, add the three following rules: #1: SSH Access - Direction: IN, Interface: net0, Macro: SSH - Secure Shell Traffic, Source: ANY, Port: Blank/Auto (Handeld by SSH), Destination: Blank/Auto (Handeld by SSH), Policy: ACCEPT, leave a comment: SSH access.
 - Then, after creating the first rule, add TCP & UDP Ports for Volt - Direction: IN, Interface: net0, Protocol: TCP or UDP (for respective rule), Source: ANY, Destination Port: 24872, Policy: ACCEPT, leave a comment: Volt server port.
 - Note: Only UDP is required for Volt; opening TCP is optional.

- SSH into the server: `ssh root@YOUR_SERVER_IP`.

- Install Docker: `apt update`, `apt install -y docker.io`, `systemctl enable --now docker`. Verify Docker installation: `docker --version`

- (Optional) Install Volt AppImage: `chmod +x Volt-Linux*.AppImage` This step is optional and only needed if you want to manage Volt locally on a VPS.

- Now, we configure the lobby itself. Run the server: `docker run -d --name volt-lobby --restart unless-stopped -p 24872:24872/udp ikuzen/yuzu-hdr-multiplayer-dedicated --room-name "My Volt Room" --password "MySecurePass2025" --max-members 8 --preferred-game "Mario Kart 8 Deluxe" --preferred-game-id "01000ABF0C84C000" --web-api-url "api.ynet-fun.xyz"` This command starts the server in the background, maps the UDP port, and sets your room settings.

- Now you can try verifying the server: `docker ps` You should see: `volt-lobby   Up   0.0.0.0:24872->24872/udp`

- Connect from the client: Open Volt on your PC, go to Multiplayer > Direct Connect to Room, enter the VPS IP address, use the room name and password you set above or alternatively you should see your lobby within the "Browse Public Game Lobby" section as well.

- Managing the Docker container: Stop the server: `docker stop volt-lobby`, Start it again: `docker start volt-lobby`, Remove it: `docker rm -f volt-lobby` if you want to change the name of your lobby, you must first stop the server via: docker stop volt-lobby, then remove it via: docker rm -f volt-lobby. Then edit the server name and just repaste the updated command.

- Notes: Only UDP port 24872 is strictly required. You can customize room name, password, max members, and preferred game. Optional parameters from the original Outcaster guide include: `--room-description` (Adds a room description in the lobby), `--allowed-name-suffix` (Restricts usernames), `--moderator-password` (Adds a moderator role), `--allow-non-preferred-game` (Allows games other than the preferred game)
