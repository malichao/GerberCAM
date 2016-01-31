# GerberCAM
**What is GerberCAM?**
GerberCAM is an opensource software to convert a PCB gerber file to a CNC machine manufacture file(G-code).The following picture shows GerberCAM’s role in PCB prototyping process.
![alt tag](https://github.com/malichao/GerberCAM/blob/master/snapshot/explain.jpg)

**What are the features of GerberCAM?**  
GerberCAM currently support following features.  
● Recieve gerber file in RS-274X format only  
● Up to two layers(top & bottom)  
● Pad holes automatic recognition  
● Tool bits(drill,conical,cylindrical) generation & management  

Contour path:  
    ● Automatically treat closed loop as contour path  
    ● Can determine contour path by thresholds  

Toolpath generating:  
    ● Single bit toolpath  
    ● Automatically DRC and show the collided path  

Pad types:  
    ● pads with rotation  
    ● Round  
    ● Rectangle  
    ● Obround  
    ● Teardrop(● Track   ● Arc)  
    
Track types:  
    ● straight line  
  
**Here are the snapshots of the current development.**
![alt tag](https://github.com/malichao/GerberCAM/blob/master/snapshot/GerberCAM_V07_Test1.png)
![alt tag](https://github.com/malichao/GerberCAM/blob/master/snapshot/GerberCAM_V07_Test2.png)
![alt tag](https://github.com/malichao/GerberCAM/blob/master/snapshot/GerberCAM_V07_Setting.png)
![alt tag](https://github.com/malichao/GerberCAM/blob/master/snapshot/GerberCAM_V07_Tool_Library.png)

GerberCAM is still a prototype.After three months of heavy development and testing it’s suspended.It has a fast generation time comparing to CopperCAM 2012,which is the CAM software I’ve been using to generate G-code for PCB prototyping for years.It can also handle some PCBs which can’t be supported by CopperCAM.  

However,since I don’t have a CNC machine to test the software anymore so I didn’t continue to work on this project.I’ve been using hundreds of gerber files and thousands of times to test all current functions.There are still a lot of testing and verification work to do in the future.If anyone is interested in this project,feel free to fork it.Also I’ve been writing a detailed document about the implementation of GerberCAM on my blog.There are several opensource CAM software projects out there and I hope my works could contribute any help.  

**Here’s the [link](http://lichaoma.com/2015/11/14/gerbercam-a-pcb-tool-path-generator/) to the document.**

