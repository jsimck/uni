export const REMOVE_EVENT_CONFIRM_SHOW = 'REMOVE_EVENT_CONFIRM_SHOW';
export const REMOVE_EVENT_CONFIRM_CLOSE = 'REMOVE_EVENT_CONFIRM_CLOSE';

export function removeEventConfirmShow(id) {
    return {
        type: REMOVE_EVENT_CONFIRM_SHOW,
        id: id
    }
}

export function removeEventConfirmClose() {
    return {
        type: REMOVE_EVENT_CONFIRM_CLOSE
    }
}