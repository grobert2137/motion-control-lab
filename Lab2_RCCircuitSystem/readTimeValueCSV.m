function [Data_t, Data_Other] = readTimeValueCSV(filename)
    % Reads a 2-column CSV (time, value) with a header row
    T = readtable(filename);      % header row becomes the column names
    Data_t     = T{:, 1};         % first column -> time
    Data_Other = T{:, 2};         % second column -> value
end
