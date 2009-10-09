function KK=amrmpm(problem_type,CFL,R1_dx)

% One dimensional MPM
% bulletproofing

if (~strcmp(problem_type, 'impulsiveBar')  && ...
    ~strcmp(problem_type, 'oscillator') && ...
    ~strcmp(problem_type, 'collidingBars') )
  fprintf('ERROR, the problem type is invalid\n');
  fprintf('     The valid types are impulsiveBar, oscillator, collidingBars\n');
  fprintf('usage:  amrmpm(problem type, cfl, R1_dx)\n');
  return;
end
%__________________________________
% hard coded domain
PPC =1;
E   =1e8;
density = 1000.;

c = sqrt(E/density);

bar_length =1.;
domain     =1.;
area       =1.;
plotSwitch = 0;

% HARD WIRED FOR TESTING
NN          = 16
R1_dx       =domain/(NN-1)

if (mod(domain,R1_dx) ~= 0)
  fprintf('ERROR, the dx in Region 1 does not divide into the domain evenly\n');
  fprintf('usage:  amrmpm(problem type, cfl, R1_dx)\n');
  return;
end

%__________________________________
% region structure 
numRegions    = int8(3);               % partition the domain into numRegions
Regions       = cell(numRegions,1);    % array that holds the individual region information

R.min         = 0;                     % location of left point
R.max         = 1/3;                   % location of right point
R.refineRatio = 1;
R.dx          = R1_dx;
R.volP        = R.dx/PPC;
R.NN          = (R.max - R.min)/R.dx;
Regions{1}    = R;

R.min         = 1/3;                       
R.max         = 2/3;
R.refineRatio = 1;
R.dx          = R1_dx/R.refineRatio;
R.volP        = R.dx/PPC;
R.NN          = (R.max - R.min)/R.dx;                  
Regions{2}    = R;

R.min         = 2/3;                       
R.max         = domain;
R.refineRatio = 1;
R.dx          = R1_dx/R.refineRatio; 
R.volP        = R.dx/PPC;
R.NN          = (R.max - R.min)/R.dx + 1;       
Regions{3}    = R;

NN = int8(0);
dt = 9999;

for r=1:numRegions
  R = Regions{r};
  NN = NN + R.NN;
  dt = min(dt, CFL*R.dx/c);
  fprintf( 'region %g, min: %g, \t max: %g \t refineRatio: %g dx: %g, dt: %g NN: %g\n',r, R.min, R.max, R.refineRatio, R.dx, dt, R.NN)
end

%__________________________________
% create particles
ip=1;
xp(1)=R1_dx/(2.*PPC);

for r=1:numRegions
  R = Regions{r};
  
  dx_P = R.dx/PPC;                             % particle dx
  
  if ~strcmp(problem_type, 'collidingBars')    % Everything except the collingBar
    while (xp(ip) + dx_P > R.min ) && ...
          (xp(ip) + dx_P < R.max ) && ...
          (xp(ip) + dx_P < bar_length)
          
      ip = ip+1;
      xp(ip)=xp(ip-1) + dx_P;
    end
  end
  
 % This needs to be fixed!!!                    % Colliding Bar
  if strcmp(problem_type, 'collidingBars')
    %left bar
    while xp(ip) + dx_P < (bar_length/2. - R.dx)
      ip=ip+1;
      xp(ip)=xp(ip-1) + dx_P;
    end

    ip=ip+1;
    xp(ip)=domain - R.dx/(2.*PPC);

    % right bar
    while xp(ip) - dx_P > (bar_length/2. + R.dx)
      ip=ip+1;
      xp(ip)=xp(ip-1) - dx_P;
    end
  end
end  % region

NP=ip  % number of particles

%__________________________________
% pre-allocate variables for speed
vol       = zeros(1,NP);
massP     = zeros(1,NP);
velP      = zeros(1,NP);
dp        = zeros(1,NP);
stressP   = zeros(1,NP);
extForceP = zeros(1,NP);
Fp        = zeros(1,NP);         

xG        = zeros(1,NN);
massG     = zeros(1,NN);
velG      = zeros(1,NN);
vel_nobc_G= zeros(1,NN);
accl_G    = zeros(1,NN);
extForceG = zeros(1,NN);
intForceG = zeros(1,NN);

BigNum    = int8(1e4);
KE        = zeros(1,BigNum);
SE        = zeros(1,BigNum);
TE        = zeros(1,BigNum);
Exact_tip = zeros(1,BigNum);
DX_tip    = zeros(1,BigNum);
TIME      = zeros(1,BigNum);


%__________________________________
% initialize other particle variables
for ip=1:NP
  [volP]    = positionToVolP(xp(ip), numRegions, Regions);
  vol(ip)   = volP;
  massP(ip) = volP*density;
  Fp(ip)    = 1.;                     % total deformation
end

%__________________________________
% Problem dependent parameters
if strcmp(problem_type, 'impulsiveBar')
  period        = sqrt(16.*bar_length*bar_length*density/E);
  TipForce      = 10.;
  D             = TipForce*bar_length/(area*E);
  M             = 4.*D/period;
  extForceP(NP) = TipForce;
end

if strcmp(problem_type, 'oscillator')
  Mass          = 10000.;
  period        = 2.*3.14159/sqrt(E/Mass);
  v0            = 0.5;
  Amp           = v0/(2.*3.14159/period);
  massP(NP)     = Mass;                    % last particle masss
  velP(NP)      = v0;                      % last particle velocity
end

if strcmp(problem_type, 'collidingBars')
  period = 4.0*dx/100.;
  for ip=1:NP
    velP(ip) =100.0;
    
    if xp(ip) > .5*bar_length
      velP(ip) = -100.0;
    end
  end
  
end

tfinal=1.0*period;

% create array of nodal locations, only used in plotting
ig = 1;
for r=1:numRegions
  R = Regions{r};
  for  c=1:R.NN
    xG(ig) = (ig-1)*R.dx;
    ig = ig + 1;
  end
end
% set up BCs
numBCs=1;

if strcmp(problem_type, 'collidingBars')
  numBCs=0;
end

BCNode(1)  = 1;
BCNode(2)  = NN;
BCValue(1) = 0.;
BCValue(2) = 1.;

t = 0.0;
tstep = 0;

%plot initial conditions
%plotResults(t,xp,dp,velP)


%==========================================================================
% Main timstep loop
while t<tfinal
  tstep = tstep + 1;
  t = t + dt;
  %fprintf('timestep %g, dt = %g, time %g \n',tstep, dt, t)

  % initialize arrays to be zero
  for ig=1:NN
    massG(ig)     =1.e-100;
    velG(ig)      =0.;
    vel_nobc_G(ig)=0.;
    accl_G(ig)    =0.;
    extForceG(ig) =0.;
    intForceG(ig) =0.;
  end
  
  % project particle data to grid
  for ip=1:NP
    [nodes,Ss]=findNodesAndWeights(xp(ip), numRegions, Regions);
    for ig=1:2
      massG(nodes(ig))     = massG(nodes(ig))     + massP(ip) * Ss(ig);
      velG(nodes(ig))      = velG(nodes(ig))      + massP(ip) * velP(ip) * Ss(ig);
      extForceG(nodes(ig)) = extForceG(nodes(ig)) + extForceP(ip) * Ss(ig);
    end
  end

  % normalize by the mass
  velG = velG./massG;
  vel_nobc_G = velG;

  % set velocity BC
  for ibc=1:numBCs
    velG(BCNode(ibc)) = BCValue(ibc);
  end

  %compute particle stress
  [stressP,vol,Fp]=computeStressFromVelocity(xp,dt,velG,E,Fp,NP,numRegions, Regions);

  %compute internal force
  for ip=1:NP
    [nodes,Gs,dx]=findNodesAndWeightGradients(xp(ip),numRegions, Regions);
    for ig=1:2
      intForceG(nodes(ig)) = intForceG(nodes(ig)) - (Gs(ig)/dx) * stressP(ip) * vol(ip);
    end
  end

  %compute the acceleration and new velocity on the grid
  accl_G    =(intForceG + extForceG)./massG;
  vel_new_G = velG + accl_G.*dt;

  %set velocity BC
  for ibc=1:numBCs
    vel_new_G(BCNode(ibc)) = BCValue(ibc);
  end

  % compute the acceleration on the grid
  for ig=1:NN
    accl_G(ig)  = (vel_new_G(ig) - vel_nobc_G(ig))/dt;
  end

  %project changes back to particles
  for ip=1:NP
    [nodes,Ss]=findNodesAndWeights(xp(ip),numRegions, Regions);
    dvelP = 0.;
    dxp   = 0.;
    
    for ig=1:2
      dvelP = dvelP + accl_G(nodes(ig))    * dt * Ss(ig);
      dxp   = dxp   + vel_new_G(nodes(ig)) * dt * Ss(ig);
    end
    
    velP(ip) = velP(ip) + dvelP;
    xp(ip)   = xp(ip) + dxp;
    dp(ip)   = dp(ip) + dxp;
  end
  
  DX_tip(tstep)=dp(NP);
  T=t; %-dt;

  %__________________________________
  % compute kinetic, strain and total energy
  KE(tstep)=0.;
  SE(tstep)=0.;
  
  for ip=1:NP
    KE(tstep) = KE(tstep) + .5*massP(ip) * velP(ip) * velP(ip);
    SE(tstep) = SE(tstep) + .5*stressP(ip) * (Fp(ip)-1.) * vol(ip);
    TE(tstep) = KE(tstep) + SE(tstep);
  end

  %__________________________________
  % compute the exact tip deflection
  if strcmp(problem_type, 'impulsiveBar')
    if(T <= period/2.)
      Exact_tip(tstep) = M*T;
    else
      Exact_tip(tstep) = 4.*D-M*T;
    end
  end
  if strcmp(problem_type, 'oscillator')
    Exact_tip(tstep) = Amp*sin(2. * 3.14159 * T/period);
  end

  TIME(tstep)=t;
  %__________________________________
  % plot intantaneous solution
  if (mod(tstep,100) == 0) && (plotSwitch == 1)
    plotResults(t, xp,dp,velP)
  end
  
  % bulletproofing
  % particles can't leave the domain
  for ip=1:NP
    if(xp(ip) >= domain) 
      t = tfinal;
      fprintf('\nparticle(%g) position is outside the domain: %g \n',ip,xp(ip))
      fprintf('now exiting the time integration loop\n\n') 
    end
  end

end

% compute on the error on the final timestep
E_err=TE(1)-TE(tstep)                        % total energy
err=abs(DX_tip(tstep)-Exact_tip(tstep))      % tip deflection

%==========================================================================
%  plot the results
plotFinalResults(TIME, DX_tip, Exact_tip, TE, problem_type, PPC, NN)

%__________________________________
%  write the results out to files
fid = fopen('particleData.dat', 'w');
fprintf(fid,'%s, PPC: %g, NN %g\n',problem_type, PPC, NN);
fprintf(fid,'p, massP, velP, stressP, extForceP, Fp\n');
for ip=1:NP
  fprintf(fid,'%g, %g, %g, %g, %g, %g\n',ip, massP(ip), velP(ip), stressP(ip), extForceP(ip), Fp(ip));
end
fclose(fid);

fid = fopen('gridData.dat', 'w');
fprintf(fid,'%s, PPC: %g, NN %g\n',problem_type, PPC, NN);
fprintf(fid,'g, xG, massG, velG, extForceG, intForceG, accl_G\n');
for ig=1:NN
  fprintf(fid,'%g, %g, %g, %g, %g, %g %g\n',ig, xG(ig), massG(ig), velG(ig), extForceG(ig), intForceG(ig), accl_G(ig));
end
fclose(fid);

end


%______________________________________________________________________
% functions
function[node, dx]=positionToNode(xp, numRegions, Regions)
  dx = -9;
  
  for r=1:numRegions
    R = Regions{r};
    if ((xp >= R.min) && (xp < R.max))
      dx = R.dx;
    end
  end
  
  node = xp/dx;
  node = floor(node) + 1;
end
%__________________________________
%
function[volP]=positionToVolP(xp, numRegions, Regions)
  volP = -9;
 
  for r=1:numRegions
    R = Regions{r};
    if ((xp >= R.min) && (xp < R.max))
      volP = R.dx;
      return;
    end
  end
end

%__________________________________
function [nodes,Ss]=findNodesAndWeights(xp, numRegions, Regions)
 
  % find the nodes that surround the given location and
  % the values of the shape functions for those nodes
  % Assume the grid starts at x=0.

  %node = xp/dx;
  %node = floor(node)+1;

  [node, dx]=positionToNode(xp, numRegions, Regions);  

  nodes(1)= node;
  nodes(2)= nodes(1)+1;

  dnode = double(node);

  locx  = (xp-dx*(dnode-1))/dx;
  Ss(1) = 1-locx;
  Ss(2) = locx;
end

%__________________________________
function [nodes,Gs, dx]=findNodesAndWeightGradients(xp, numRegions, Regions);
 
  % find the nodes that surround the given location and
  % the values of the shape functions for those nodes
  % Assume the grid starts at x=0.

  %node = xp/dx;
  %node = floor(node)+1;

  [node, dx]=positionToNode(xp,numRegions, Regions);

  nodes(1) = node;
  nodes(2) = nodes(1)+1;

  %dnode = double(node);

  %locx=(xp-dx*(dnode-1))/dx;
  Gs(1) = -1;
  Gs(2) = 1;
end
%__________________________________
function [stressP,vol,Fp]=computeStressFromVelocity(xp,dt,velG,E,Fp,NP, numRegions, Regions)
                                                                                
  for ip=1:NP
    [nodes,Gs,dx] = findNodesAndWeightGradients(xp(ip), numRegions, Regions);
    [volP]        = positionToVolP(xp(ip), numRegions, Regions);
    gUp=0;
    for ig=1:2
      gUp = gUp + velG(nodes(ig)) * (Gs(ig)/dx);
    end

    dF          =1. + gUp * dt;
    Fp(ip)      = dF * Fp(ip);
    stressP(ip) = E * (Fp(ip)-1);
    vol(ip)     = volP * Fp(ip);
  end
end

%__________________________________
function plotResults(t, xp, dp, velP)

  close all;
  set(gcf,'position',[100,100,900,900]);

  % particle velocity vs position
  subplot(2,1,1),plot(xp,velP,'bx');

  tmp = sprintf('Time %g',t);
  title(tmp);
  xlabel('Particle Position');
  ylabel('velocity')

  % particle ???? vs position
  subplot(2,1,2),plot(xp,dp,'b-');
  ylabel('dp');
  input('hit return');
end

%__________________________________
function plotFinalResults(TIME, DX_tip, Exact_tip, TE, problem_type, PPC, NN)

  close all;
  set(gcf,'position',[100,100,900,900]);

  % tip displacement vs time
  subplot(3,1,1),plot(TIME,DX_tip,'bx');

  hold on;
  subplot(3,1,1),plot(TIME,Exact_tip,'r-');

  tmp = sprintf('%s, ppc: %g, NN: %g',problem_type, PPC, NN);
  title(tmp);
  legend('Simulation','Exact')
  xlabel('Time [sec]');
  ylabel('Tip Deflection')

  %total energy vs time
  subplot(3,1,2),plot(TIME,TE,'b-');
  ylabel('Total Energy');


  % compute error
  err=abs(DX_tip-Exact_tip);

  subplot(3,1,3),plot(TIME,err,'b-');
  ylabel('Abs(error)')
end

