// Status bar network activity actions, only for IOS
export const SHOW_QR_READER = 'SHOW_QR_READER';
export const CLOSE_QR_READER = 'CLOSE_QR_READER';
export const SET_QR_READER_READ = 'SET_QR_READER_READ';

export function showQrReader() {
    return {
        type: SHOW_QR_READER
    };
}

export function closeQrReader() {
    return {
        type: CLOSE_QR_READER
    };
}

export function setQrReaderRead(value) {
    return {
        type: SET_QR_READER_READ,
        read: value
    };
}