// Push Notification action
export const QR_MODAL_OPEN = 'QR_MODAL_OPEN';

export function openQrModal(data) {
    return {
        type: QR_MODAL_OPEN,
        data: data
    };
}

// Remove Notification action
export const QR_MODAL_CLOSE = 'QR_MODAL_CLOSE';

export function closeQrModal() {
    return {
        type: QR_MODAL_CLOSE
    };
}