# Blockchain-omnet-project
Implementation of blockchain in omnet++ environment

# My first simulation
to start the simulation as intended you have to copy the myapplication folder in the OverSim/src/applications in omnet++. Then you have to copy next lines in
the /OverSim/simulations/omnetpp.ini file:
```
[Config Kademlia]
description = Kademlia (SimpleUnderlayNetwork)
*.underlayConfigurator.churnGeneratorTypes = "oversim.common.LifetimeChurn"
**.overlayType = "oversim.overlay.kademlia.KademliaModules"
**.tier1Type = "oversim.applications.myapplication.MyApplicationModules"
**.networkLayer.networkProtocolType = "Flood"
**.targetOverlayTerminalNum = 5
**.enableDrops = false
**.dropChance = 0
**.sendPeriod = 1s
**.numToSend = 1
**.largestKey = 5
```
Then all you have to do is start the simulation.
