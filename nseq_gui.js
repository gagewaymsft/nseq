// guiding principle: don't worry about doing anything better than anyone else. just do it simpler.

var ctx;
var canvas;
var isClick = false;
var showNodes = true;
var showTracks = false;
var showSeq = false;
var pointerX = 0;
var pointerY = 0;
var winWidth = 100;
var winHeight = 100;
var engine;
var nodeManager;

// ignore this stuff, it was a hacky idea that i need to restructure
// begin ignore

function addNode() {
    return ["NODE","CLOSE"];
}

function close() {
    return ["CLOSE"];
}

//end ignore

class nseqClickableElement{
    constructor(x,y,w,h,bg,fg,content,rootAction,subActions)
    {
        this.x = x;
        this.y = y;
        this.w = w;
        this.h = h;
        this.bg = bg,
        this.fg = fg;
        this.content = content;
        this.rootAction = rootAction;
        this.subActions = subActions;
    }
    
    isClicked(x,y)
    {
        let fullResponse = [];
        let response = [];
        for(let i = 0; i < this.subActions.length; i++)
        {
            let response = this.subActions[i].isClicked(x,y);
            if(response[0] != ["PASS"])
            {
                for(let i = 0; i < response.length; i++)
                {
                    fullResponse.push(response[i]);
                }
                return fullResponse;
            }
        }
        if(x > this.x && x < this.x + this.w && y > this.y && this.y < this.y + this.h)
        {
            response = this.clickAction(x,y);
            if(response[0] != ["PASS"])
            {
                for(let i = 0; i < response.length; i++)
                {
                    fullResponse.push(response[i]);
                }
                return fullResponse;
            }
        }
        return ["PASS"];
    }
    
    clickAction(x,y)
    {
        if(this.rootAction == "move")
        {
            return ["MOVE"];
        }

        return this.rootAction();
    }
    
    draw(x,y)
    {
        // this has to be restructured, the "content" should be like intro text / a header
        // the inner widgets should be relative, maybe automatically grid?
        // some rules based vector system that looks simple and averagely good
        
        drawWindow(this.x, this.y, this.w, this.h, this.bg, this.fg, this.content);            
        for(let i = 0; i < this.subActions.length; i++)
        {
            this.subActions[i].draw(this.x,this.y);
        }
    }
}

class nseqTrack {
    constructor(id) {
    }
        // is this just a type of node?
}

class nseqSequence {
    constructor(id) {
    }
        // is this just a type of node?
}

class nseqNode extends nseqClickableElement{
    constructor(id, midiInCh, midiOutCh, mediaInCh, mediaOutCh, sideCh, x, y, w, h, bg, fg, content, rootAction,subActions) {
        super(x,y,w,h,bg,fg,content,rootAction,subActions);
        // is everything a node? like a compressor has a sub node for each stage that impacts the signal?
        // if it's nodes all the way down there could eventually be a cool 3d display of signal flow
        // people love that shit
        // would this be easier to maintain longterm or harder if all things derive from a single class?
        // i lean towards easier
        // it would also mean custom nodes could be stored as json files for import / export
        // theoretically infinite expandability and a universal plugin format!

        // if all data takes the form of either midi or media, a midi cc maxes out at 127
        // for data that needs to be passed around with higher precision we can add CCs
        // cc0 and val0 sometimes have meaning so best to ignore them
        // 127 values * 127 ccs = 16219
        // 127 values * 127 ccs * 16 channels = 258064
        // that's a lot of precision

        this.id = id;
        this.midiInCh = midiInCh;
        this.midiOutCh = midiOutCh;
        this.mediaInCh = mediaInCh;
        this.mediaOutCh = mediaOutCh;
        this.sideCh = sideCh;

        this.mediaInSlots = [];
        this.mediaOutSlots = [];
        this.midiInSlots = [];
        this.midiOutSlots = [];
        this.sidechainSlots = [];

    }
    
    draw(x,y)
    {
        super.draw(x,y);
        // this all needs to be vector based
        this.drawInputs();
        this.drawOutputs();
        this.drawSidechain();
    }
    
    drawInputs()
    {
        console.log("drawing inputs", this.midiInCh);
        for(let i = 0; i < this.midiInCh; i++)
        {
            //draw an input patch point in the midi color on the left of box
            ctx.strokeStyle = this.fg;
            ctx.beginPath();
            ctx.arc(this.x + 8, 12 + this.y + 12 * i, 4, 0, 2 * Math.PI);
            ctx.stroke();
        }
        for(let i = 0; i < this.mediaInCh; i++)
        {
            //draw an input patch point in the media color on the left of box
        }
    }
    
    drawOutputs()
    {
        for(let i = 0; i < this.midiOutCh; i++)
        {
            //draw an output patch point in the midi color on the right of box
            ctx.strokeStyle = this.fg;
            ctx.beginPath();
            ctx.arc(this.x + this.w - 8, 12 + this.y + 12 * i, 4, 0, 2 * Math.PI);
            ctx.stroke();
        }
        for(let i = 0; i < this.mediaOutCh; i++)
        {
            //draw an output patch point in the media color on the right of box
        }
    }
    
    drawSidechain()
    {
        for(let i = 0; i < this.sideCh.length; i++)
        {
            //draw a sidechain patch point in the media color at the bottom
        }
    }
}

class nseqNodeManager {
    constructor() {
        this.nodes = [];
    }
    
    addNode(node)
    {
        this.nodes.push(node);
    }
}

class nseqWidget extends nseqClickableElement{
    constructor(x,y,w,h,bg,fg,content,rootAction,subActions) {
        super(x,y,w,h,bg,fg,content,rootAction,subActions);
        // it seems like bloat to have things that don't have audio or midi passthrough be nodes
        // unclear if or how this is different from base class though but it seems like it might need to be?
    }
}

class nseqGraphicsEngine {
    constructor() {
        this.widgets = [];
    }
    
    refresh()
    {
        ctx.fillStyle = "#080808";
        ctx.fillRect(0,0,winWidth,winHeight);
        if(showNodes)
        {
            this.drawNodes();
        }
        else if(showTracks)
        {
            this.drawTracks();
        }
        else if(showSeq)
        {
            this.drawSeq();
        }
        this.drawWidgets();
        
    }

    removeWidget()
    {
        console.log("removing widget");
        return true;
    }
    
    addWidget(x,y,type)
    {
        // this all should be vector based and way better
        // inner widgets need to inherit position relative to main window but i was lazy
        
        let widget;
        if(type == 'addNode')
        {
            // this should actually have a dropdown menu and not just the word "type"
            let w = 128;
            let h = 96;
            let xStart = x - w/2; 
            let yStart = y - h/2;
            let bg = "#aaaaaa";
            let fg = "#111111";
            let yesAction = new nseqWidget(xStart+12,yStart+h-32,20,20,fg,bg,"y",addNode,[]);
            let noAction = new nseqWidget(xStart+44,yStart+h-32,20,20,fg,bg,"n",close,[]);
            let rootAction = "move";
            widget = new nseqWidget(xStart, yStart,w,h,bg,fg,"add track?", rootAction, [yesAction, noAction]);
        }
        this.widgets.push(widget);
        this.refresh();
    }
    
    drawWidgets()
    {
        for(let i = 0; i < this.widgets.length; i++)
        {
            this.widgets[i].draw();
        }
    }
    
    addNode(x,y,type)
    {
        let node;
        if(type == 'track')
        {
            let w = 128;
            let h = 96;
            let xStart = x - w/2; 
            let yStart = y - h/2;
            let bg = "#aaaaaa";
            let fg = "#843d00";
            let rootAction = "move";
            node = new nseqNode(0, 4, 1, 1, 1, 1, xStart, yStart,w,h,bg,fg,"track 0", rootAction, []);
        }
        this.widgets.push(node);
        this.refresh();
    }
    
    drawNode(node)
    {
        for(let i = 0; i < this.nodes.length; i++)
        {
            this.nodes[i].draw();
        }
    }
    
    drawNodes()
    {
        for(let i = 0; i < nodeManager.nodes.length; i++)
        {
            if (nodeManager.nodes[i].isOpen)
            {
                drawNode(nodeManager.nodes[i]);
            }
        }
        
        //node-based view
    }
    
    isClicked(x,y)
    {
        let clicked = false;
        for(let i = 0; i < this.widgets.length; i++)
        {
            let response = this.widgets[i].isClicked(x,y);
//            console.log(response[0]);
            if(response[0] != "PASS")
            {
                let sortedResponse = [];
                for(let j = 0; j < response.length; j++)
                {
                    if(response[j] == "CLOSE")
                    {
                        sortedResponse.unshift(response[j]);
                    }
                    else
                    {
                        sortedResponse.push(response[j]);
                    }                    
                }
                for(let j = sortedResponse.length - 1; j >= 0; j--)
                {
                    console.log(sortedResponse[j]);
                    if(sortedResponse[j] == "CLOSE")
                    {
                        console.log("closing window");
                        this.widgets.splice(i, 1);
                        return true;
                    }
                    if(sortedResponse[j] == "NODE")
                    {
                        console.log(this.widgets[i]);
                        this.addNode(this.widgets[i].x, this.widgets[i].y, 'track');
                    }
                }
                return true;
            }
        }
    }
    
    isNode(x,y)
    {
        for(let i = 0; i < nodeManager.nodes.length; i++)
        {
            if (x > nodeManager.nodes[i].x && x , nodeManager.nodes[i].x + nodeManager.nodes[i].w && y > nodeManager.nodes[i].y && y < nodeManager.nodes[i].y + nodeManager.nodes[i].h)
            {
                return true;
            }
        }
        return false;
    }
    
    drawTracks()
    {
        //vertical ableton / famitracker style clip-based / mixer view
    }
    
    drawSeq()
    {
        //horizontal pro-tools style sequencer-based view
    }
    
    // what if everything is a node including tracks and sequencers
    // and track or sequencer display mode is just a template that's all track nodes etc
    // mix and match gui modes as is useful
}

function initGUI() {
    canvas = document.getElementById("main");
    nodeManager = new nseqNodeManager();
    ctx = canvas.getContext("2d");
    engine = new nseqGraphicsEngine();
    resizeCanvas();
    window.addEventListener('resize', resizeCanvas, false);
    canvas.addEventListener('mousedown', (e) => {
        getCursorPosition(canvas, e);
        leftClickActions();
    });
    canvas.addEventListener('mousemove', (e) => {
        getCursorPosition(canvas, e);
        move();
    });
    canvas.addEventListener('mouseup', (e) => {
        getCursorPosition(canvas, e);
        endClickActions();
    });
    console.log("starting up.");
}

function resizeCanvas()
{
    winWidth = window.innerWidth * .9;
    winHeight = window.innerHeight * .9;
    canvas.width = winWidth;
    canvas.height = winHeight;
    updateCanvas();
}

const getCursorPosition = (canvas, event) => {
  const rect = canvas.getBoundingClientRect();
  pointerX = event.clientX - rect.left;
  pointerY = event.clientY - rect.top;
}

function leftClickActions() {
    if(!engine.isClicked(pointerX,pointerY))
    {
        engine.addWidget(pointerX, pointerY, "addNode");
    }
    else
    {
        engine.refresh();
    }
}

function move() {
}

function endClickActions() {
}

function updateCanvas() {
    engine.refresh();
}

function drawWindow(x, y, w, h, bg, fg, content)
{
        ctx.fillStyle = "#77777722";
        ctx.fillRect(x + 16,y + 16,w,h);
        ctx.fillStyle = bg;
        ctx.fillRect(x,y,w,h);
        
        ctx.font = "16px Courier";
        ctx.fillStyle = fg;
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        ctx.fillText(content, (x + x + w) / 2, (y + y + h) / 2);        
}
