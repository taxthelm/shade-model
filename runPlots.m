elevationPlot;
minElev = min(min(b));
maxElev = max(max(b));
elevDiff = maxElev - minElev;
m = [0:9.17:2053*9.17];
n = [0:9.17:1965*9.17];
for k=18:30
    eval(sprintf('shadingPlot_%d',k));
    c = b - elevDiff * a;
    close(figure);
    %{
    col1Light = [0.87:0.13/128:1]';
    col2Light = [0.49:0.46/128:0.95]';
    col3Light = [0:0.87/128:0.87]';
    col1Dark = [0.08:0.92/128:1]';
    col2Dark = [0.17:-0.17/128:0]';
    col3Dark = [0.55:0.45/128:1]';
    %}
    %set(gcf, 'ColorMap', [col1, col2, col3])
    
    hours = floor(k / 4);
    minutes = mod(k * 15, 60);
    colormap(cat(1,winter(256), autumn(256)));
    h = surf(m, n, b, c);
    title(sprintf('Shaded Surface Map | TIME : %2d:%02d', hours, minutes));
    xlabel('West - East');
    ylabel('North - South');
    zlabel('Elevation (m)');
    view(-35, 25);
    axis equal
    axis vis3d
    set(gca, 'CLim', [minElev - elevDiff, maxElev]);
    set(h,'LineStyle','none')
    saveas(h, sprintf('gradientShadePlot_%02d',k),'jpg');
    close(figure);
end
