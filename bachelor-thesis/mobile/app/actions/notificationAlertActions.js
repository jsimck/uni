// Alert actions
export const PUSH_ALERT = 'PUSH_ALERT';
export const CLOSE_ALERT = 'CLOSE_ALERT';

export function pushAlert(title, message) {
    return {
        type: PUSH_ALERT,
        title: title,
        message: message
    };
}

export function closeAlert() {
    return {
        type: CLOSE_ALERT
    };
}