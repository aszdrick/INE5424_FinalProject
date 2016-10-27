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
        Q_export:     out std_logic_vector(ADDRESS_WIDTH - 1 downto 0)
    );
end avalon_adapter;

architecture Structure of avalon_adapter is
    signal reg_command: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal reg_address: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal reg_data_in: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    --signal useless_holder: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal load:        std_logic;

begin

    --Q_export <= useless_holder;
    Q_export(31 downto 29) <= (others => '0');
    Q_export(28 downto 24) <= sem_status;
    Q_export(23 downto 16) <= sem_data_out(7 downto 0);
    Q_export(15 downto 12) <= reg_command(3 downto 0);
    Q_export(11 downto 8) <= reg_address(3 downto 0);
    Q_export(7 downto 0) <= reg_data_in(7 downto 0);
    -- input receiving
    process (clock, resetn)
    begin
        if resetn = '0' then
                reg_command  <= (others => '0');
                reg_data_in  <= (others => '0');
                reg_address  <= (others => '0');
                --useless_holder <= (others => '0');
                load         <= '0';
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
                                load <= '1';
                            when others => null;
                        end case;
                    elsif (write = '0' and read = '1') then
                            case address is
                                when "00" =>
                                    readdata(ADDRESS_WIDTH - 1 downto 5) <= (others => '0');
                                    readdata(4 downto 0) <= sem_status;
                                    
                                when "01" =>
                                    readdata <= sem_data_out;
                                when others => null;
                            end case;
                    else    
                        load <= '0';
                    end if;
                else    
                    load <= '0';
                end if;
            end if;
        end if;
    end process;
    
    -- Input processing
    process (clock)
    begin
        if (rising_edge(clock)) then
            if (load = '1') then
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