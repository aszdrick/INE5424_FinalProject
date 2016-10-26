library ieee;
use ieee.std_logic_1164.all;
entity Qsys_Semaphore is
    generic (
        MAX_SEMAPHORES: integer := 8; -- Must lower than 2^COUNTER_WIDTH
        SEM_FIFO_SIZE: integer := 4;
        COUNTER_WIDTH: integer := 8;
        ADDRESS_WIDTH: integer := 32
    );
    port (
        clock: in std_logic;
        resetn: in std_logic;
        read: in std_logic;
        write: in std_logic;
        chipselect: in std_logic;
        address: in std_logic_vector(1 downto 0);
        writedata: in std_logic_vector(31 downto 0);
        readdata: out std_logic_vector(31 downto 0)
    );
end Qsys_Semaphore;

architecture Structure OF Qsys_Semaphore is
    signal conn_command: std_logic_vector(2 downto 0);
    signal conn_address: std_logic_vector(COUNTER_WIDTH - 1 downto 0);
    signal conn_data_in: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal conn_data_out: std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
    signal conn_status: std_logic_vector(4 downto 0);
    
    component semaphore is
        generic (
            C_MAX_SEMAPHORES: integer := 8;
            C_FIFO_SIZE: integer := 4;
            C_VALUE_WIDTH: integer := 8;
            C_FIFO_WIDTH: integer := 32
        );
        port (
            p_clk: in std_logic;
            p_rst: in std_logic;
            p_cmd: in std_logic_vector(2 downto 0);
            p_addr: in std_logic_vector(C_VALUE_WIDTH - 1 downto 0);
            p_data_in: in std_logic_vector(C_FIFO_WIDTH - 1 downto 0);
            p_data_out: out std_logic_vector(C_FIFO_WIDTH - 1 downto 0);
            p_status: out std_logic_vector(4 downto 0)
        );
    end component;

    component avalon_adapter is
        generic (
            COUNTER_WIDTH: integer := 8;
            ADDRESS_WIDTH: integer := 32
        );
        port (
            -- input from avalon MM interface
            clock: in std_logic;
            resetn: in std_logic;
            read: in std_logic;
            write: in std_logic;
            chipselect: in std_logic;
            address: in std_logic_vector(1 downto 0);
            -- input from user
            writedata: in std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
            -- outputs from semaphore
            sem_status: in std_logic_vector(4 downto 0);
            sem_data_out: in std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
            -- inputs to semaphore
            sem_command: out std_logic_vector(2 downto 0);
            sem_address: out std_logic_vector(COUNTER_WIDTH - 1 downto 0);
            sem_data_in: out std_logic_vector(ADDRESS_WIDTH - 1 downto 0);
            -- outputs to user
            readdata: out std_logic_vector(ADDRESS_WIDTH - 1 downto 0)
        );
    end component;
    
begin
    Interface_block: avalon_adapter 
        generic map (
            COUNTER_WIDTH => COUNTER_WIDTH,
            ADDRESS_WIDTH => ADDRESS_WIDTH
        )
        port map (
            clock => clock,
            resetn => resetn,
            read => read,
            write => write,
            chipselect => chipselect,
            address => address,
            writedata => writedata,
            readdata => readdata,
            sem_status => conn_status,
            sem_data_out => conn_data_out,
            sem_command => conn_command,
            sem_address => conn_address,
            sem_data_in => conn_data_in
        );

    Semaphore_block: semaphore
        generic map (
            C_MAX_SEMAPHORES => MAX_SEMAPHORES,
            C_FIFO_SIZE => SEM_FIFO_SIZE,
            C_VALUE_WIDTH => COUNTER_WIDTH,
            C_FIFO_WIDTH => ADDRESS_WIDTH
        )
        port map (
            p_clk => clock,
            p_rst => resetn,
            p_cmd => conn_command,
            p_addr => conn_address,
            p_data_in => conn_data_in,
            p_data_out => conn_data_out,
            p_status => conn_status
        );
end Structure;