
Todos:
    toolpath generating(using single bit):
        toolpath of every net,toolpath of a total layer,toolpath collision check
    tool library:
        creating a bit,bit choosing
    multiple bit toolpath generating(based on tool library)
    DRC:
        DRC at opening a gerber file(base on tool library)
    Adding centering holes(horizontal/vertical/quadruple)
    smart drill hole generating(need drill library)
    Postprocessing:
        Drill file output
        topLayer output
        bottomLayer output(mirror)
        routing output

    most of running time are dominated by intersect calculation,not data read/write/move

=============test result=============
Net calculation time of hengliu.gtl:
230 KB,17338 line
v0.6----1839 ms
v0.5----3062 ms
v0.42--13984 ms
v0.4---50221 ms
v0.3---xxxxxxxx
=============test result=============


○○○○○○○○○○○○○○○○○○○○○○○○○○○○○ Features ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○
● support        ○ not support

● recieve gerber file in RS-274X format only
● up to two layers(top & bottom)

contour path:
    ● automatically treat closed loop as contour path
    ● can determine contour path by thresholds

toolpath generating:
    ● single bit toolpath
    ● automatically DRC and show the collided path

pad types:
    ● pads with rotation
    ● Round
    ● Rectangle
    ● Obround
    ● Teardrop(● Track   ● Arc)
    ○ Thermal
    ○ Octagonal
    
track types:
    ● straight line
    ○ curve line

○○○○○○○○○○○○○○○○○○○○○○○○○○○○○ Features ○○○○○○○○○○○○○○○○○○○○○○○○○○○○○


---------------------------------gerberCAM v0.7---------------------------------
add treeview
add setting dialog,tool library
add hole recognize,drawing
todo:    tool select


---------------------------------gerberCAM v0.65---------------------------------
todo:improve geber instruction interpreting(G36,G37,AM,rotary...)
fix drawPCB bug in v0.64


---------------------------------gerberCAM v0.64---------------------------------
todo:improve geber instruction interpreting(G36,G37,AM,rotary...)
drawPCB.cpp:
    when drawing a pad,using the original point as center point

gerber.cpp:
    use boundingRect to determine borderRect(to be tested)

a bug in toolpath.cpp:can be found using ro5.gtl


---------------------------------gerberCAM v0.63---------------------------------
add threshold to define contours(calculating boundingRect area)
paint contour
paint the collided toolpath nets


---------------------------------gerberCAM v0.62---------------------------------
automatically recognize contour as a routing toolpath
toolpath collision detect
fix preprocess.cpp:
    sameNet()--mergeRect() error
    sameNet()--removeAt() error



---------------------------------gerberCAM v0.61---------------------------------
successfully generated the toolpath
fix preprocess.cpp&toolpath.cpp:
    fix the calculation error in trackToMyRect method



---------------------------------gerberCAM v0.6---------------------------------
replace all float data with integer
speed is improved due to int type
start using clipper library
bug:
    netList number is not the same as previous versions(now is less)
        and this bug appears only on some gerberfiles(pcb3.gtl)


---------------------------------gerberCAM v0.5---------------------------------
    start to generate toolpath
toolpath.cpp
    generated toolpath of every single element



---------------------------------gerberCAM v0.42---------------------------------
add preprocess.cpp to handle gerber data first

bug:
    gerber.h:different order of variables in struct will cause corrupt
    toolpath::sameNet():cant use g.padsList.at(i).block.boundingRect;


mainwindow.cpp:
    added coordinate display at statusbar
    improve zoom in/out action

preprocess.cpp:
    move toolpath to preprocess.cpp
    add boundingRect to every Net,largely reduce time!sepecially for 10k+ segments file
toolpath.cpp:
    adding bounding rect to reduce calculation

fix toolpath.cpp:
    fix a bug in net categorization
    floating point data comparision error

fix:
    zoom in/out problem

fix drawPCB.cpp:
    fix boundingRect error,display normal now


---------------------------------gerberCAM v0.3---------------------------------
start to calculate toolpath
toolpath.cpp
    finished track&track collision detect
    finished track&pad collision detect
    finished pad&pad collision detect
    finished net categorization

fix gerber.cpp:
    gerber trackNum count error
    gerber borderRect error

gerber.cpp:
    add block count to reduce net calculation


---------------------------------gerberCAM v0.2---------------------------------
add a drawPCB class to take care of drawing layer
Obround pad is drawn by a short line with roundcap instead of ellipse
import 2 layers
fit in view


---------------------------------gerberCAM v0.1---------------------------------
gerberfile: basic interpret
one layer display

