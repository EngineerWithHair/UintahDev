%_________________________________
% 09/01/04   --Todd
% This matLab script generates 1D plots
% of piston validation data and makes a movie
%
%  NOTE:  You must have 
%  setenv MATLAB_SHELL "/bin/tcsh -f"
%  in your .cshrc file for this to work
%_________________________________

close all;
clear all;
clear function;

%________________________________
% USER INPUTS
uda = 'AMR_HotBlob.uda.000'

desc = '1D hotBlob';
legendText = {'L-0','L-1'}
pDir = 1;                    
symbol = {'+','*r'}
mat      = 0;


numPlotCols = 1;
numPlotRows = 4;

plotRho   = true;
plotTemp  = true;           
plotPress    = true;
plotVol_frac = false;
plotSp_vol   = true
plotVel      = false; 

% lineExtract start and stop
startEnd ={'-istart 0 0 0 -iend 100 0 0';'-istart 0 0 0 -iend 100 0 0' };

%________________________________
%  extract the physical time for each dump
c0 = sprintf('puda -timesteps %s | grep : | cut -f 2 -d":" >& tmp',uda)
[status0, result0]=unix(c0);
physicalTime  = importdata('tmp');
nDumps = length(physicalTime) - 1;

  
%set(0,'DefaultFigurePosition',[0,0,1024,768]);
%_________________________________
% Loop over all the timesteps
for(ts = 1:nDumps )
  ts = input('input timestep') 
  time = sprintf('%d sec',physicalTime(ts+1));
  
  %find max number of levels
  c0 = sprintf('puda -gridstats %s -timesteplow %i -timestephigh %i |grep "Number of levels" | grep -o \\[0-9\\] >& tmp',uda, ts,ts);
  [s, maxLevel]=unix(c0);
  maxLevel  = importdata('tmp');
  levelExists{1} = 0;
   
  % find what levels exists
  for(L = 2:maxLevel)
    c0 = sprintf('puda -gridstats %s -timesteplow %i -timestephigh %i >& tmp ; grep "Level: index %i" tmp',uda, ts,ts, L-1);
    [levelExists{L}, result0]=unix(c0);
  end 
  
  %______________________________
  for(L = 1:maxLevel) 
    level = L-1;
    if (levelExists{L} == 0) 
     
      S_E = startEnd{L};
      unix('/bin/rm -f press temp rho vel sp_vol vol_frac');
      plotNum = 1;
      level
      
      %______________________________
      %  temperature   
      if plotTemp
        c1 = sprintf('lineextract -v temp_CC -l %i -cellCoords -timestep %i %s -o temp -m %i  -uda %s',level,ts,S_E,mat,uda);
        [s1, r1]   = unix(c1);
        temp1{1,L} = importdata('temp');
        
        x = temp1{1,L}(:,pDir)
        subplot(numPlotRows,numPlotCols,plotNum), plot(x,temp1{1,L}(:,4),symbol{L})
        %xlim ([0 50])
        %axis([0 5 295 320])
        xlabel('x')
        ylabel('temp')
        title(time);
        grid on;
        hold
        plotNum = plotNum+ 1;
      end
      %______________________________
      % pressure
      if plotPress
        c2 = sprintf('lineextract -v press_CC -l %i -cellCoords -timestep %i %s -o press -m 0 -uda %s',level,ts,S_E,uda);
        [s2, r2]    = unix(c2);
        press1{1,L} = importdata('press');
        
       
        subplot(numPlotRows,numPlotCols,plotNum),plot(x,press1{1,L}(:,4),symbol{L})
        %xlim ([0 50])
        %axis([0 5 101000 109000])
        xlabel('x')
        ylabel('pressure')
        title(desc);
        grid on;
        hold
        plotNum = plotNum+ 1;
      end
      %_____________________________
      %  Density
      if plotRho
        c3 = sprintf('lineextract -v rho_CC -l %i -cellCoords -timestep %i %s -o rho  -m %i  -uda %s',level,ts,S_E,mat,uda);
        [s3, r3]  = unix(c3);
        rho1{1,L} = importdata('rho');
        
        
        subplot(numPlotRows,numPlotCols,plotNum), plot(x,rho1{1,L}(:,4),symbol{L})
        %xlim ([0 50])
        %axis([0 5 1.75 1.9])
        xlabel('x')
        ylabel('rho')
        grid on;
        hold
        plotNum = plotNum+ 1;
      end
      %____________________________
      %   sp_vol
      if plotSp_vol
        c4 = sprintf('lineextract -v sp_vol_CC -l %i -cellCoords -timestep %i %s -o sp_vol -m %i  -uda %s',level,ts,S_E,mat,uda);
        [s4, r4] = unix(c4);
        f1{1,L}  = importdata('sp_vol');
        
        
        subplot(numPlotRows,numPlotCols,plotNum), plot(x,f1{1,L}(:,4),symbol{L})
        %xlim ([0 50])
        %axis([0 50 0 0.01])
        ylabel('sp vol');
        grid on;
        hold
        plotNum = plotNum+ 1;
      end
      %____________________________
      %  velocity
      if plotVel
        c5 = sprintf('lineextract -v vel_CC      -l %i -cellCoords -timestep %i %s -o vel_tmp  -m %i  -uda %s',level,ts,S_E,mat,uda);
        [s5, r5]=unix(c5);
        % rip out [ ] from velocity data
        c6 = sprintf('sed ''s/\\[//g'' vel_tmp | sed ''s/\\]//g'' >vel');
        [s6, r6]  = unix(c5);
        vel1{1,L} = importdata('vel');
        
        subplot(numPlotRows,numPlotCols,plotNum), plot(x, vel1{1,L}(:,5),symbol{L})
        legend('y');
        %xlim ([0 50])
        %axis([0 5 -10 10])
        ylabel('vel CC');
        grid on;
        plotNum = plotNum+ 1;
      end
      %____________________________
      %   vol_frac
      if plotVol_frac
        c6 = sprintf('lineextract -v vol_frac_CC -l %i -cellCoords -timestep %i %s -o vol_frac -m %i  -uda %s',level,ts,S_E,mat,uda);
        [s6, r6]=unix(c6);
        vf{1,L} = importdata('vol_frac');
        
        
        subplot(numPlotRows,numPlotCols,plotNum), plot(x,vf{1,L}(:,4),symbol{L})
        %xlim ([0 50])
        %axis([0 5 -10 10])
        ylabel('vol frac');
        grid on;
        hold
        plotNum = plotNum+ 1;
      end
    end  % if level exists
 end
end
%__________________________________
% show the move and make an avi file
%hFig = figure;
%movie(hFig,M,1,3)
%movie2avi(M,'delT.avi','fps',5,'quality',100);
%clear all
