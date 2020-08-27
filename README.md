# hidclient

My tweaks to [hidclient](http://anselm.hoffmeister.be/computer/hidclient/index.html.en).

Original software is GPL'ed, written by Anselm Martin Hoffmeister.

## Modifications Introduced by This Fork

This fork adds support for taking keyboard-input (and keyboard-input only)
from the current TTY instead of the input/event devices. This means
that there is no interoperability problem with X; no tweaks are necessary
and it is trivial to use `hidclient` in a terminal window while using other
applications normally. Only text typed in the terminal window is sent
to `hidclient`.

### Usage

A new option `-t` was added which tells `hidclient` to read from the current
terminal (`/dev/tty`) and ignore all event devices.
I also found that all the SDP stuff is hopelessly out-of-sync with current
the DBUS implementation and does not work. Luckily, the application (maybe
only once paired?) works just fine without SDP, i.e., I always use the `-s`
option:

      sudo hidclient -s -t

does the job. Once hidclient is up I have to restart bluetooth. However, 
this is not as painful as the original documentation suggests. On my Ubuntu
18.04LTS system I can just disable and then re-enable bluetooth from the
desktop/GUI and afterwards I can connect my phone without problems.

Disconnecting the bluetooth connection and reconnecting is not a problem
at all. There is no need to restart bluetooth, it just works.

The only thing I have to make sure is that I restart bluetooth
*after starting `hidclient`*. Once `hidclient` and then bluetooth
are up I have no more issues.

### HID Protocol Notes

Playing with this software I realized that the HID protocol is extremely
primitive and dumb. E.g., there is no way (in 2020?) to send UTF-8. Otherwise,
we could automatically support a huge set of characters. Instead, what the
HID protocol supports is plain old PS1-keyboard scan-codes. I.e., any
character typed on the computer's keyboard has to be mapped to a PS1 code.

This is bad news for non-ASCII (international) users. Old PCs used to
be built with the same hardware, supplying the same scan-codes everywhere
but manufacturers would simply paint different symbols in different countries
on the keys.

This means that you have to switch the keyboard layout on the target
device (e.g., tell the phone that you are using a German BT-keyboard).
Then, when the target device receives the PS1 scan-code for ';' it
interprets that as a 'ö' -- since that's what a German keyboard features
at the physical location where the US keyboard has a ';'.

In order to provide at least minimal support for international characters
I have added mappings for the few characters that can be encoded with
the ALT modifier on PS1:

á,é,í¸ó,ú,ä,ö,ü,ñ and their upper-case variants. If the terminal sends
UTF-8 sequences for these characters then `hidclient` will map then to
the corresponding ALT-PS1 characters.

In a similar fashion basic ANSI escape sequences that are used by
the termial (mostly for cursor movement) are mapped to the PS1 UP, DOWN,
LEFT, RIGHT, HOME, END etc. keys.

### Original README

## Current status

Currently, this works, but it's definitely not a smooth setup process. Quirks:

- Requires `--compat` flag to `bluetoothd`
    - (possibly unneeded if not publishing SDP records)
- Only works once per `bluetoothd` session
    - Need to restart `bluetooth` service on host after disconnecting client
- `bluetoothd` needs to be restarted *after* `hidclient` is started

## Full example usage

This is what works for me running Arch Linux on both machines.

### One-time setup steps

On host machine:

    ## compile this software
    [host hidclient]$ make

    ## Edit /usr/lib/systemd/system/bluetooth.service to change this line:
    ExecStart=/usr/lib/bluetooth/bluetoothd

    ## To:
    ExecStart=/usr/lib/bluetooth/bluetoothd --compat

    ## Determine correct IDs for use below:
    [host hidclient]$ XAUTHORITY=$HOME/.Xauthority sudo ./hidclient -l

### Each time connecting

On the system running hidclient, in the source directory:

    ## The numbers provided to the -e flag(s) will vary for your system
    [host hidclient]$ XAUTHORITY=$HOME/.Xauthority sudo ./hidclient -x -e0 -e17 -e18
      HID keyboard/mouse service registered
      Opened /dev/input/event0 as event device [counter 0]
      Opened /dev/input/event17 as event device [counter 1]
      Opened /dev/input/event18 as event device [counter 2]
      The HID-Client is now ready to accept connections from another machine

On the system running hidclient, in another terminal emulator:

    [host ~]$ sudo systemctl restart bluetooth.service
    [host ~]$ bluetoothctl
      [bluetooth]# power on
      Changing power on succeeded
      [bluetooth]# agent on
      Agent registered
      [bluetooth]# default-agent
      Default agent request successful
      [bluetooth]#

On the client system:

    [client ~]$ bluetoothctl
      [bluetooth]# power on
      Changing power on succeeded
      [bluetooth]# agent on
      Agent registered
      [bluetooth]# default-agent
      Default agent request successful
      [bluetooth]# connect XX:XX:XX:XX:XX:XX
      Attempting to connect to XX:XX:XX:XX:XX:XX
      [CHG] Device XX:XX:XX:XX:XX:XX Connected: yes
      Connection successful
      [host]#
