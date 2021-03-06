library ieee;
use ieee.std_logic_1164.all;
entity avalon_adapter is
    generic (
        COUNTER_WIDTH: integer := 8;
        ADDRESS_WIDTH: integer := 32
    );
    port (
        -- input from avalon MM interface
        clock:        in std_logic;
        resetn:       in std_logic;
        read:         in std_logic;
        write:        in std_logic;
        chipselect:   in std_logic;
        address:      in std_logic_vector(1 downto 0);
        -- input from user
        writedata:    in std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
        -- outputs from semaphore
        sem_status:   in std_logic_vector(4 downto 0);
        sem_data_out: in std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
        -- inputs to semaphore
        sem_command:  out std_logic_vector(2 downto 0);
        sem_address:  out std_logic_vector(COUNTER_WIDTH - 1 downto 0);
        sem_data_in:  out std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
        -- outputs to user
        readdata:     out std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
        -- debug outputs
        Q_export:     out std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
        Led_export: out std_logic_vector(COUNTER_WIDTH + 7 downto 0)
    );
end avalon_adapter;

architecture Structure of avalon_adapter is
    signal reg_command: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal reg_address: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal reg_data_in: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal data_received: std_logic;

begin
    -- debug outputs
    Led_export(4 downto 0) <= sem_status;
    Led_export(7 downto 5) <= reg_command(2 downto 0);
    Led_export(COUNTER_WIDTH + 7 downto 8) <= reg_address(COUNTER_WIDTH - 1 downto 0);
    Q_export(31 downto 16) <= reg_data_in(15 downto 0);
    Q_export(15 downto 0) <= sem_data_out(15 downto 0);

    process (clock, resetn)
    begin
        if resetn = '0' then
                reg_command <= (others => '0');
                reg_data_in <= (others => '0');
                reg_address <= (others => '0');
                data_received <= '0';
        else 
            if (rising_edge(clock)) then
                if (chipselect = '1') then
                    if (write = '1' and read = '0') then
                        case address is
                            when "00" =>
                                reg_command <= writedata;
                            when "01" =>
                                reg_address <= writedata;
                            when "10" =>
                                reg_data_in <= writedata;
                                data_received <= '1';
                            when others => null;
                        end case;
                    elsif (write = '0' and read = '1') then
                        data_received <= '0';
                            case address is
                                when "00" =>
                                    readdata(ADDRESS_WIDTH - 1 downto 5) <= (others => '0');
                                    readdata(4 downto 0) <= sem_status;
                                when "01" =>
                                    readdata <= sem_data_out;
                                when others => null;
                            end case;
                    else    
                        data_received <= '0';
                    end if;
                else    
                    data_received <= '0';
                end if;
            end if;
        end if;
    end process;

    process (clock)
    begin
        if (rising_edge(clock)) then
            if (data_received = '1') then
                sem_command <= reg_command(2 downto 0);
                sem_address <= reg_address(COUNTER_WIDTH - 1 downto 0);
                sem_data_in <= reg_data_in;
            else
                sem_command <= (others => '0');
                sem_address <= (others => '0');
                sem_data_in <= (others => '0');
            end if;
        end if;
    end process;
END Structure;