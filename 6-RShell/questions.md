1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

_To ensure that the remote client has received or send the full output or command from the server, we can send in EOF character to indicate the end of a message. To ensure the full output has been sent, there can be checks in places, where the length of the message sent and received is compared to make sure the entire thing is sent over._

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

_There can be EOF characters added to the front and back of the message. If that character is encountered, then one would know that they are at the starting or ending points of the message. Issue with handling this would be regarding memory constraints. If we add more characters to our message, we need to make sure memory is properly allocated to handle this scenario._

3. Describe the general differences between stateful and stateless protocols.

_Stateful protocols are slower with more overhead, while stateless protocols are faster with less overhead._

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

_UDP is unreliable, but it can package up to 64k bytes together. There would be no issue regarding acknowledging the start and end of a message stream with UDP._

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

_Socket is the interface/abstraction provided by the operating system to enable applications to use network communications._