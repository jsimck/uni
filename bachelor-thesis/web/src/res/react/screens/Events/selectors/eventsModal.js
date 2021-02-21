import { createStructuredSelector } from 'reselect'

export const eventsModalSelector = createStructuredSelector({
    processing: state => state.events.processing,
    visible: state => state.editModal.visible,
    event: state => state.editModal.event
});