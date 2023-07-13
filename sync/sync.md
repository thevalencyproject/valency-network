# How Syncing Works
Syncing interfaces with the local-save framework to provide both syncing and file management functions.
Syncing works differently for both nodes and wallets:

**Future Updates**
Some future updates include, but are not limited to:
1. Using LZMA Compression and Decompression when sending a large amount of blocks from server to client.
2. Saving the raw string data of the latest blocks to send - this will decrease network latency.
3. Dynamically reducing the wait threshold based on various network statistics - this would decrease network latency and increase network efficiency.
4. Algorithm optimization (block selection, string conversion, etc.) - this will increase efficiency and reduce network latency.

Nodes: Act as both a server and client, acting as a server to fulfill requests, and acting as a client to send requests to other nodes.

Clients: Act as a client, only sending requests to the nodes
<br><br>
**NOTE: Both server and client are run in their own threads.**
