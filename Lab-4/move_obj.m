function move_obj
% Use serial data to communicate with Arduino and tell it to move arm in
% manner predetermined by image processing video from camera input.

% *************************************************************************
%    Set up Video input device
% *************************************************************************
    vid = videoinput('winvideo',1,'YUY2_640x480');
    triggerconfig(vid,'manual');
    set(vid,'FramesPerTrigger',1 );
    set(vid,'TriggerRepeat', Inf);
    set(vid,'ReturnedColorSpace','rgb');

    
    
% *************************************************************************
%    Tweak your preferences
% *************************************************************************
    thresh_color_g = 20;
     thresh_color_r = 70;
    thresh_area = 20;
    thresh_grab = 99; 
    
    
% *************************************************************************
%    Set up communication with arduino
% *************************************************************************
    

%    
    arduino=serial('COM11','BaudRate',9600); % create serial communication object on port COM4   
    fopen(arduino); % initiate arduino communication


% *************************************************************************
%    Start execution of program
% *************************************************************************
    
    start(vid); %Starts camera device
    dist = thresh_grab + 1;
    
    
    % Change this loop structure according to your logic. You could keep a
    % for loop here for known number of iterations or end loop if you want
    % a flexible condition
    count=1;
    radius=0;
    thetafinal=0;
    theta=0;
    while count < 11
        count = count +1;
        trigger(vid);
        pause(.5)
        im = getdata(vid,1);
        figure(1);
        
        imshow(im); % Shows image in a figure

        % Store separate RGB values in matrices
        r = im(:,:,1);
        g = im(:,:,2);
        b = im(:,:,3);

        % The given values are used to make a Green and a Red selection
        % respectively by ignoring other remaining colors.
        % Values may be changed according to environmental factors. If
        % extremely interested in how/why, please ask later.
        justSelection_g = g - r/2 - b/2;
%          figure(1) ; 
%         imshow(justSelection_g);
        justSelection_r = r - g/2 - b/2;
        % Converting to binary black/white for clarity and decision making.
        black_white_g = justSelection_g > thresh_color_g;
        black_white_r = justSelection_r > thresh_color_r;
        
%         figure(2);
%         imshow(black_white_g);
        colormap(gray);
        
        % Removes smaller (noise) areas and concentrates on the major
        % object of choice only. To get good values, keep thresh larger;
        % but be careful.
        object_g_area = bwareaopen(black_white_g, thresh_area);
        object_r_area = bwareaopen(black_white_r, thresh_area);
%         figure(1);       
%         imshow(object_r_area);
%         figure(3);   
%         imshow(object_g_area);
%         figure(4);   
%         imshow(object_r_area);
        % Obtain centroid locations and respective areas of major object
        % groups of respective colors from image.
        stats_g = regionprops(object_g_area, {'centroid', 'area'});
        stats_r = regionprops(object_r_area, {'centroid', 'area'});
        
        [~, id_g] = max([stats_g.Area]);
        [~, id_r] = max([stats_r.Area]);
        
        if (id_g > 0) & (id_r > 0) % Please don't change & to && even though MATLAB tells you to
            
            % X and Y coordinates of centres 
            x_g = stats_g(id_g).Centroid(1);
            y_g = stats_g(id_g).Centroid(2);
            x_r = stats_r(id_r).Centroid(1);
            y_r = stats_r(id_r).Centroid(2);

            % Calculate distance between centres
            dist = sqrt((x_r-x_g)^2 + (y_r-y_g)^2);
            dist = dist*25/320;
            theta = 95-atand((x_g-x_r)/(y_g-y_r));
            %dist 17 - 25
            %Uncomment this if you want to look at your centres
%             figure(1);
%             hold on
%             imshow(im);
%             plot([x_g, x_r], [y_g, y_r],'*');
%             text(540,400,num2str(dist));
%             hold off 
        end
        radius =  radius +dist;
       
        thetafinal=thetafinal+theta;
        % *****************************************************************
        %    Rest of your logic for giving control instructions to Arduino
        %    goes here.
        % *****************************************************************
         % end communication with arduino
        % break;
        
        
    end
       radius =  radius/10
       thetafinal = (thetafinal/10)
       str1 = num2str(floor(thetafinal));
       str2 = num2str(floor(radius));
       str3 = strcat(str1,str2)
      fwrite(arduino, str3);
      % pause(1);
      % send answer variable content to arduino
      %fwrite(arduino, num2str(radius));  
     fclose(arduino); 

    stop(imaqfind); % Necessary operation for closing program
    
end