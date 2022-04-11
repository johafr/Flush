# Flush
Flush shell implementation

TODO: 
- implementere separering med tab
- Fikse return signal med execvp for "&"
- implementere catch zombies i 3.4
  - Må sette fin = 1 når en process er ferdig utført. Skal bruke WNOHANG mest sannsynlig.
  - Mån få implementert kill() til å terminere child process ikke parent