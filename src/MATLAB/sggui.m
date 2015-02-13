function sggui(varargin)
panelColor=get(0,'DefaultUIcontrolBackgroundColor');

set(0,'Units','characters');
screenSize=get(0,'ScreenSize');
fSize=[0 0 300 70];
if screenSize(4)<fSize(4)
    fSize(4)=screenSize(4)-15;
end
if screenSize(3)<fSize(3)
    fSize(3)=screenSize(3)-15;
end
fPos=[(screenSize(3:4)-fSize(3:4))/2,fSize(3:4)];

% Figure
figuretitle='Stochastic Game Analysis Tool';
f=figure('Units','characters',...
    'Position',fPos,...
    'Name',figuretitle,...
    'Color',panelColor,...
    'NumberTitle','off',...
    'MenuBar','none',...
    'Toolbar','figure',...
    'Resize','on',...
    'ResizeFcn',@fResizeFcn);

% Panels
botPanelHeight=10;
botPanel = uipanel(f,'Units','characters',...
    'Position',[0 0 fSize(3) botPanelHeight],...
    'BackgroundColor',panelColor,...
    'BorderType','none',...
    'ResizeFcn',@botPanelResizeFcn);
leftPanel = uipanel(f,'Units','characters',...
    'Position',[0 botPanelHeight fSize(3)/2 (fPos(4)-botPanelHeight)],...
    'BackgroundColor',panelColor,...
    'BorderType','none',...
    'ResizeFcn',@leftPanelResizeFcn);
rightPanel = uipanel(f,'Units','characters',...
    'Position',[fSize(3)/2 botPanelHeight fSize(3)/2 (fPos(4)-botPanelHeight)],...
    'BackgroundColor',panelColor,...
    'BorderType','none');

% Axes
mainAx = axes('parent',rightPanel);

% Table
payoffTable = uitable('Parent',leftPanel,...
    'Units','characters',...
    'Position',[1 1 fSize(3)/2-3 (fPos(4)-botPanelHeight)-4],...
    'BackgroundColor',[1 1 1;1 1 1],...
    'FontSize',13,...
    'Data',{},...
    'CellEditCallback',@payoffTableEditCallback,...
    'ColumnEditable',[true true]);

% Sliders
set(botPanel,'Units','pixels');
botPanelPos=get(botPanel,'Position');
iterslider = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','pixels',...
    'BackgroundColor',panelColor,...
    'Position',[175 botPanelPos(4)-40 botPanelPos(3)/2-180,20],...
    'Enable','on',...
    'Callback',@sliderCallback);
stateslider = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','pixels',...
    'BackgroundColor',panelColor,...
    'Position',[175 botPanelPos(4)-80 botPanelPos(3)/2-180,20],...
    'Enable','on',...
    'Callback',@sliderCallback);
actslider = uicontrol('Parent',botPanel,...
    'Style','slider',...
    'Units','pixels',...
    'BackgroundColor',panelColor,...
    'Position',[175 botPanelPos(4)-120 botPanelPos(3)/2-180,20],...
    'Enable','on',...
    'Callback',@sliderCallback);

% Labels and text boxes to show the current valuations
set(botPanel,'Units','characters');
botPanelPos = get(botPanel,'Position');
itertext = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','iter:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[1,botPanelHeight-4,10,2.5]);
statetext = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','state:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[1,botPanelHeight-7,10,2.5]);
acttext = uicontrol('Parent',botPanel,...
    'Style','text',...
    'String','act:',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[1,botPanelHeight-10,10,2.5]);
iteredit = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[12,botPanelHeight-3.5,20,2]);
statedit = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[12,botPanelHeight-6.5,20,2]);
actedit = uicontrol('Parent',botPanel,...
    'Style','edit',...
    'Enable','off',...
    'Units','characters',...
    'HorizontalAlignment','right',...
    'FontSize',13,...
    'Position',[12,botPanelHeight-9.5,20,2]);

% File menu
fileMenu = uimenu(f,'Label','File');
openMenu = uimenu(fileMenu,'Label','Open...',...
    'Callback',@openCallback); %#ok<*NASGU>
saveMenu = uimenu(fileMenu,'Label','Save...',...
    'Callback',@saveCallback);

% Display menu
displayMenu = uimenu(f,'Label','Display');
fixzoomMenu = uimenu(displayMenu,'Label','Fix zoom...',...
    'checked','off',...
    'callback',@checkMenuCallback);
plotOptionsMenu = uimenu(displayMenu,'Label','Plot options...');
GstartMenu = uimenu(plotOptionsMenu,'Label','Initial set...');
GstartVertMenu = uimenu(GstartMenu,'Label','Vertices...',...
    'Checked','off',...
    'Callback',@plotOptionsMenuCallback);
GstartBndMenu = uimenu(GstartMenu,'Label','Boundary...',...
    'Checked','off',...
    'Callback',@plotOptionsMenuCallback);
GMenu = uimenu(plotOptionsMenu,'Label','Feasible payoffs in current state...');
GVertMenu = uimenu(GMenu,'Label','Vertices...',...
    'Checked','on',...
    'Callback',@plotOptionsMenuCallback);
GBndMenu = uimenu(GMenu,'Label','Boundary...',...
    'Checked','on',...
    'Callback',@plotOptionsMenuCallback);
WMenu = uimenu(plotOptionsMenu,'Label','Current W...');
WVertMenu = uimenu(WMenu,'Label','Vertices...',...
    'Checked','off',...
    'Callback',@plotOptionsMenuCallback);
WBndMenu = uimenu(WMenu,'Label','Boundary...',...
    'Checked','on',...
    'Callback',@plotOptionsMenuCallback);
WtempMenu = uimenu(plotOptionsMenu,'Label','Next W...');
WtempVertMenu = uimenu(WtempMenu,'Label','Vertices...',...
    'Checked','on',...
    'Callback',@plotOptionsMenuCallback);
% WtempBndMenu = uimenu(WtempMenu,'Label','Boundary...',...
%     'Checked','off',...
%     'Callback',@plotOptionsMenuCallback);
EwMenu = uimenu(plotOptionsMenu,'Label','Expected continuation values...');
EwVertMenu = uimenu(EwMenu,'Label','Vertices...',...
    'Checked','off',...
    'Callback',@plotOptionsMenuCallback);
EwBndMenu = uimenu(EwMenu,'Label','Boundary...',...
    'Checked','on',...
    'Callback',@plotOptionsMenuCallback);
contvalsMenu = uimenu(plotOptionsMenu,'Label','Continuation values...',...
    'Checked','on',...
    'Callback',@plotOptionsMenuCallback);


%% Variables
% Scope the arrays to store data.

plotOpts = struct('GstartVert',false,'GstartBnd',false,...
    'GVert',true,'GBnd',true,...
    'WVert',false,'WBnd',true,...
    'IC',true,...
    'EwVert',false,'EwBnd',true,...
    'WtempVert',true,'WtempBnd',false,...
    'newpointsVert',true,'newpointsBnd',false,...
    'contvalsVert',true,'contvalsBnd',false);
plotStyles = struct('GstartVert','k.','GstartBnd','k-',...
    'GVert','k.','GBnd','k-',...
    'WVert','b.','WBnd','b-',...
    'IC','g-',...
    'EwVert','r.','EwBnd','r-',...
    'WtempVert','b.','WtempBnd','b--',...
    'newpointsVert','bo','newpointsBnd','b-',...
    'contvalsVert','ro','contvalsBnd','r--',...
    'g','mx');

newdata = load('sgexample2.mat');
sg=newdata.sg;

Gstart = vertrep(cell2mat({sg.G.points}')); Gstart.convhull;

[xlim,ylim] = findlim(Gstart.points);

set(iterslider,'sliderstep',[1 1]/(numel(sg.testoutput)-1),...
    'value',0);
set(stateslider,'sliderstep',[1 1]/(sg.nstates-1),...
    'value',0);
set(actslider,'sliderstep',[1 1]/(numel(sg.testoutput(sg.currentiter).children(sg.currentstate).children)-1),...
    'value',0);

defaultsavefile=sprintf('newsg_%s.mat',date);

plotData();

%% Helper functions
    function plotData()
        to=sg.currentto;
        
        nnewpoints=size(to.newpoints,1);
        
        GstartVert=zeros(0,2);
        GstartBnd=zeros(0,2);
        GVert=zeros(0,2);
        GBnd=zeros(0,2);
        WVert=zeros(0,2);
        WBnd=zeros(0,2);
        ICh=zeros(0,2);
        ICv=zeros(0,2);
        EwVert=zeros(0,2);
        EwBnd=zeros(0,2);
        WtempVert=zeros(0,2);
        WtempBnd=zeros(0,2);
        newpointsBnd=zeros(0,2);
        newpointsVert=zeros(0,2);
        contvalsVert=zeros(0,2);
        contvalsBnd=zeros(0,2);
        if plotOpts.GstartVert
            GstartVert=Gstart.ch([1:end 1],:);
        end
        if plotOpts.GstartBnd
            GstartBnd=Gstart.ch([1:end 1],:);
        end
        if plotOpts.GVert
            GVert=sg.G(sg.currentstate).ch([1:end,1],:);
        end
        if plotOpts.GBnd
            GBnd=sg.G(sg.currentstate).ch([1:end,1],:);
        end
        if plotOpts.WVert
            WVert=to.W.ch([1:end,1],:);
        end
        if plotOpts.WBnd
            WBnd=to.W.ch([1:end,1],:);
        end
        if plotOpts.EwVert && ~isempty(to.Ew)
            EwVert=to.Ew.ch([1:end 1],:);
        end
        if plotOpts.EwBnd && ~isempty(to.Ew)
            EwBnd=to.Ew.ch([1:end 1],:);
        end
        if plotOpts.WtempVert && ~isempty(to.Wtemp)
            WtempVert=to.Wtemp(1:(to.filledupto-nnewpoints),:);
        end
        if plotOpts.WtempBnd ~isempty(to.Wtemp)
            WtempBnd=to.Wtemp(1:(to.filledupto-nnewpoints),:);
        end
        if plotOpts.newpointsVert
            newpointsVert=to.newpoints;
        end
        if plotOpts.newpointsBnd
            newpointsBnd=to.newpoints;
        end
        if plotOpts.contvalsVert
            contvalsVert=to.contvals;
        end
        if plotOpts.contvalsBnd
            contvalsBnd=to.contvals;
        end
        
        if strcmp(get(fixzoomMenu,'checked'),'on')
            xlim=get(mainAx,'xlim');
            ylim=get(mainAx,'ylim');
        else
            [xlim,ylim] = findlim([GstartVert;GstartBnd;WVert;WBnd;EwVert;EwBnd;GVert;GBnd;WtempVert;WtempBnd;newpointsVert;newpointsBnd;contvalsVert;contvalsBnd]);
        end
        
        if plotOpts.IC && sg.currentactc<length(sg.currentto.viableacts)
            ICh=[to.IC(1),to.IC(2);xlim(2) to.IC(2)];
            ICv=[to.IC(1),to.IC(2);to.IC(1) ylim(2)];
        end
                
        plot(mainAx,...
            GstartVert(:,1),GstartVert(:,2),plotStyles.GstartVert,...
            GstartBnd(:,1),GstartBnd(:,2),plotStyles.GstartBnd,...
            GVert(:,1),GVert(:,2),plotStyles.GVert,...
            GBnd(:,1),GBnd(:,2),plotStyles.GBnd,...
            WVert(:,1),WVert(:,2),plotStyles.WVert,...
            WBnd(:,1),WBnd(:,2),plotStyles.WBnd,...
            EwVert(:,1),EwVert(:,2),plotStyles.EwVert,...
            EwBnd(:,1),EwBnd(:,2),plotStyles.EwBnd,...
            ICh(:,1),ICh(:,2),plotStyles.IC,...
            ICv(:,1),ICv(:,2),plotStyles.IC,...
            WtempVert(:,1),WtempVert(:,2),plotStyles.WtempVert,...
            WtempBnd(:,1),WtempBnd(:,2),plotStyles.WtempBnd,...
            newpointsVert(:,1),newpointsVert(:,2),plotStyles.newpointsVert,...
            newpointsBnd(:,1),newpointsBnd(:,2),plotStyles.newpointsBnd,...
            contvalsVert(:,1),contvalsVert(:,2),plotStyles.contvalsVert,...
            contvalsBnd(:,1),contvalsBnd(:,2),plotStyles.contvalsBnd,...
            to.g(:,1),to.g(:,2),plotStyles.g);
        
        for intsctncounter=1:size(to.contvals,1)
            line([to.contvals(intsctncounter,1);to.g(1)],[to.contvals(intsctncounter,2);to.g(2)],'color','m','linestyle',':');
        end

        axis equal;
        set(mainAx,'xlim',xlim,'ylim',ylim);
        
        set(findobj('color','m'),'MarkerSize',14);
        if sg.currentactc<=length(to.viableacts)
            title(gca,sprintf('Iter %d, state %d, act %d, transition probabilities ( %s, %s, %s )',...
                sg.currentiter,sg.currentstate,to.viableacts(sg.currentactc),...
                strtrim(rats(to.pi(1))),strtrim(rats(to.pi(2))),strtrim(rats(to.pi(3)))));
        else
            title(gca,sprintf('Iter %d, state %d, final',sg.currentiter,sg.currentstate));
        end
        
        set(payoffTable,'data',formatdata(sg.G1{sg.currentstate},sg.G2{sg.currentstate}));
        
    end % plotData


%% Callbacks
    function sliderCallback(src,event)
        if isempty(sg.testoutput)
            return
        end
        
        newiter = round(1+get(iterslider,'value')*(numel(sg.testoutput)-1));
        newstate = round(1+get(stateslider,'value')*(sg.nstates-1));
        newactc = round(1+get(actslider,'value')*(numel(sg.testoutput(newiter).children(newstate).children)-1));
        
        sg.currentiter=newiter;
        sg.currentstate=newstate;
        
        if src~=actslider
            set(actslider,'sliderstep',[1 1]/(numel(sg.testoutput(sg.currentiter).children(sg.currentstate).children)-1),...
                'value',0);
            sg.currentactc=1;
        else
            sg.currentactc=newactc;
        end
                
        set(iteredit,'string',sprintf('%d',sg.currentiter));
        set(statedit,'string',sprintf('%d',sg.currentstate));
        if sg.currentactc<=length(sg.currentto.viableacts)
            set(actedit,'string',sprintf('%d',sg.currentto.viableacts(sg.currentactc)));
        else
            set(actedit,'string',sprintf('Final'));
        end
        plotData();
    end % sliderCallback

    function checkMenuCallback(src,event)
        switch get(src,'checked')
            case 'on'
                set(src,'checked','off');
            case 'off'
                set(src,'checked','on');
        end
    end % checkMenuCallback

    function openCallback(src,event)
        [filename,pathname]=uigetfile('*.mat','Pick a file');
        if filename
            % If filename is 0, the user hit cancel. 
            try
                newdata = load([pathname filename]);
                sg = newdata.sg;
                
                set(iterslider,'sliderstep',[1 1]/(numel(sg.testoutput)-1),...
                    'value',0);
                set(stateslider,'sliderstep',[1 1]/(sg.nstates-1),...
                    'value',0);
                set(actslider,'sliderstep',[1 1]/(numel(sg.testoutput(sg.currentiter).children(sg.currentstate).children)-1),...
                    'value',0);
                
                defaultsavefile=filename;
            catch ME
                errordlg(sprintf('Not able to open %s%s. Following error message retrieved:\n\n%s',...
                    pathname,filename,...
                    ME.getReport),...
                    'Unable to open');
            end
        end
    end % openCallback

    function saveCallback(src,event)
        [filename,pathname]=uiputfile(defaultsavefile,'Pick a file');
        if filename
            % If filename is 0, the user hit cancel. 
            try
                save([pathname filename],'cea');

                set(f,'Name',[figuretitle ': ' filename ', ' num2str(size(cea.distributions,1)) ' equilibria']);
            catch ME
                errordlg(sprintf('Not able to save to %s%s. Following error message retrieved:\n\n%s',...
                    pathname,filename,ME.getReport),'Unable to save');
            end
        end
    end % saveCallback

    function plotOptionsMenuCallback(src,event)
        checkMenuCallback(src,event);
        
        plotOpts = struct('GstartVert',false,'GstartBnd',false,...
            'GVert',false,'GBnd',false,...
            'WVert',false,'WBnd',false,...
            'IC',true,...
            'EwVert',false,'EwBnd',false,...
            'WtempVert',false,'WtempBnd',false,...
            'newpointsVert',true,'newpointsBnd',false,...
            'contvalsVert',false,'contvalsBnd',false);

        if strcmp(get(GstartVertMenu,'checked'),'on')
            plotOpts.GstartVert=true;
        end
        if strcmp(get(GstartBndMenu,'checked'),'on')
            plotOpts.GstartBnd=true;
        end
        if strcmp(get(GVertMenu,'checked'),'on')
            plotOpts.GVert=true;
        end
        if strcmp(get(GBndMenu,'checked'),'on')
            plotOpts.GBnd=true;
        end
        if strcmp(get(WVertMenu,'checked'),'on')
            plotOpts.WVert=true;
        end
        if strcmp(get(WBndMenu,'checked'),'on')
            plotOpts.WBnd=true;
        end
        if strcmp(get(EwVertMenu,'checked'),'on')
            plotOpts.EwVert=true;
        end
        if strcmp(get(EwBndMenu,'checked'),'on')
            plotOpts.EwBnd=true;
        end
        if strcmp(get(WtempVertMenu,'checked'),'on')
            plotOpts.WtempVert=true;
        end
%         if strcmp(get(WtempBndMenu,'checked'),'on')
%             plotOpts.WtempBnd=true;
%         end
        if strcmp(get(contvalsMenu,'Checked'),'on')
            plotOpts.contvalsVert=true;
        end
        
        plotData();
    end % plotOptionsMenuCallback


%% Resize functions
    function fResizeFcn(src,event)
        set([f botPanel leftPanel rightPanel],...
            'Units','characters');
        fPos=get(f,'Position');
        set(botPanel,'Position',[0 0 fPos(3) botPanelHeight]);
        set(leftPanel,'Position',[0 botPanelHeight fPos(3)/2 (fPos(4)-botPanelHeight)]);
        set(rightPanel,'Position',[fPos(3)/2 botPanelHeight fPos(3)/2 (fPos(4)-botPanelHeight)]);
    end % resizeFig

    function leftPanelResizeFcn(src,event)
        set([payoffTable,leftPanel],'Units','characters');
        leftPanelPos=get(leftPanel,'Position');
        set(payoffTable,'Position',[1 1 leftPanelPos(3)-3 leftPanelPos(4)-4]);
    end % leftPanelResizeFcn

    function botPanelResizeFcn(src,event)
        set([botPanel iterslider stateslider actslider],'Units','pixels');
        botPanelPos=get(botPanel,'Position');
        set(iterslider,'Position',[200 botPanelPos(4)-40 botPanelPos(3)/2-230,20]);
        set(stateslider,'Position',[200 botPanelPos(4)-80 botPanelPos(3)/2-230,20]);
        set(actslider,'Position',[200 botPanelPos(4)-120 botPanelPos(3)/2-230,20]);

        % Labels and text boxes to show the current valuations
        set([botPanel itertext statetext acttext iteredit statedit actedit],...
            'Units','characters');
        botPanelPos = get(botPanel,'position');
        set(itertext,'Position',[1,botPanelHeight-4,10,2.5]);
        set(statetext,'Position',[1,botPanelHeight-7,10,2.5]);
        set(acttext,'Position',[1,botPanelHeight-10,10,2.5]);
        set(iteredit,'Position',[12,botPanelHeight-3.5,20,2]);
        set(statedit,'Position',[12,botPanelHeight-6.5,20,2]);
        set(actedit,'Position',[12,botPanelHeight-9.5,20,2]);
    end % botPanelResizeFcn


end % sggui

function [xlim,ylim] = findlim(X)
% Finds good limits for the display for coordinates listed in Y,
% compares with old limits.

if isempty(X)
    xlim=[];
    ylim=[];
    return;
end

scale = 10;
minmax = [min(X)',max(X)'];
minmax = minmax + bsxfun(@times,(minmax(:,2)-minmax(:,1))/scale,[-1 1;-1 1]);

xlim = minmax(1,:); ylim=minmax(2,:);
end

function [m1,m2]=parsedata(x)
% Parses the data stored as cell strings of the form (x1,x2)
m1=nan(size(x)); m2=m1;
for i=1:numel(x)
    [m1(i),m2(i)]=strread(x{i},'%f%f','delimiter',',');
end
end

function x=formatdata(m1,m2)
% Formats the matrix as cell strings
x=cell(size(m1));
for i=1:numel(m1)
    x{i}=[num2str(m1(i),4) ',' num2str(m2(i),3)];
end
end