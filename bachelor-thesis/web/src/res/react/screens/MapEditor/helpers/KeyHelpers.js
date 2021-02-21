import _ from 'lodash'

const KeyHelpers = {
    getCurrentOs: () => {
        if (_.includes(navigator.appVersion, 'Win')) { return 'windows'; }
        if (_.includes(navigator.appVersion, 'Mac')) { return 'mac'; }
        if (_.includes(navigator.appVersion, 'X11')) { return 'unix'; }
        if (_.includes(navigator.appVersion, 'Linux')) { return 'linux'; }
    },
    getCtrlKeyCode: () => {
        // Returns ctrl or CMD key code based on current OS, CMD = 91, CTRL = 17
        if (KeyHelpers.getCurrentOs() == 'mac') {
            return 91;
        } else {
            return 17;
        }
    },
    getCtrlKeyLabel: () => {
        if (KeyHelpers.getCurrentOs() == 'mac') {
            return 'CMD';
        } else {
            return 'Ctrl';
        }
    }
};

export default KeyHelpers