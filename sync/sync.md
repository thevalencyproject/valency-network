# How Syncing Works
Syncing interfaces with the local-save framework to provide both syncing and file management functions.
Syncing works differently for both nodes and wallets:

Nodes: Act as both a server and client, acting as a server to fulfill requests, and acting as a client to send requests to other nodes.

Clients: Act as a client, only sending requests to the nodes
<br><br>
**NOTE: Both server and client are run in their own threads.**
