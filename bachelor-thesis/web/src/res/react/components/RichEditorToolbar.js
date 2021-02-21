import React, { Component } from 'react'
import { ButtonGroup, Button, MenuItem, DropdownButton, OverlayTrigger, Tooltip } from 'react-bootstrap';

class RichEditor extends Component
{
    render() {
        const tooltipStyles = <Tooltip id="rich-editor-button-styles">Styly bloku</Tooltip>;
        const tooltipCode = <Tooltip id="rich-editor-button-code">Kód</Tooltip>;
        const tooltipStrikethrough = <Tooltip id="rich-editor-button-strikethrough">Přeškrtnuté</Tooltip>;
        const tooltipBold = <Tooltip id="rich-editor-button-bold">Tučné</Tooltip>;
        const tooltipItalic = <Tooltip id="rich-editor-button-italic">Kurzíva</Tooltip>;
        const tooltipUnderline = <Tooltip id="rich-editor-button-underline">Podtržení</Tooltip>;
        const tooltipOrderedList = <Tooltip id="rich-editor-button-ordered-list">Seznam</Tooltip>;
        const tooltipUndo = <Tooltip id="rich-editor-button-undo">Zpět</Tooltip>;
        const tooltipRedo = <Tooltip id="rich-editor-button-redo">Dopředu</Tooltip>;
        const tooltipLink = <Tooltip id="rich-editor-button-link">Odkaz</Tooltip>;

        return (
            <ButtonGroup className="editor-buttons">
                <OverlayTrigger placement="top" overlay={tooltipStyles}>
                    <DropdownButton
                        id="editor-buttons-block-types"
                        bsStyle="link"
                        title={<i className="fa fa-fw fa-header"/>}
                    >
                        <MenuItem onClick={() => this.props.onToggleBlockType('unstyled')}>p</MenuItem>
                        <MenuItem onClick={() => this.props.onToggleBlockType('code-block')}><pre>pre</pre></MenuItem>
                        <MenuItem onClick={() => this.props.onToggleBlockType('header-one')}><h1>h1</h1></MenuItem>
                        <MenuItem onClick={() => this.props.onToggleBlockType('header-two')}><h2>h2</h2></MenuItem>
                        <MenuItem onClick={() => this.props.onToggleBlockType('header-three')}><h3>h3</h3></MenuItem>
                    </DropdownButton>
                </OverlayTrigger>
                <OverlayTrigger placement="top" overlay={tooltipCode}>
                    <Button bsStyle="link" onClick={() => this.props.onToggleInlineStyle('CODE')}>
                        <i className="fa fa-fw fa-code"/>
                    </Button>
                </OverlayTrigger>
                <OverlayTrigger placement="top" overlay={tooltipStrikethrough}>
                    <Button bsStyle="link" onClick={() => this.props.onToggleInlineStyle('STRIKETHROUGH')}>
                        <i className="fa fa-fw fa-strikethrough"/>
                    </Button>
                </OverlayTrigger>

                <OverlayTrigger placement="top" overlay={tooltipBold}>
                    <Button bsStyle="link" onClick={() => this.props.onToggleInlineStyle('BOLD')}>
                        <i className="fa fa-fw fa-bold"/>
                    </Button>
                </OverlayTrigger>
                <OverlayTrigger placement="top" overlay={tooltipItalic}>
                    <Button bsStyle="link" onClick={() => this.props.onToggleInlineStyle('ITALIC')}>
                        <i className="fa fa-fw fa-italic"/>
                    </Button>
                </OverlayTrigger>
                <OverlayTrigger placement="top" overlay={tooltipUnderline}>
                    <Button bsStyle="link" onClick={() => this.props.onToggleInlineStyle('UNDERLINE')}>
                        <i className="fa fa-fw fa-underline"/>
                    </Button>
                </OverlayTrigger>

                <OverlayTrigger placement="top" overlay={tooltipOrderedList}>
                    <Button bsStyle="link" onClick={() => this.props.onToggleBlockType('unordered-list-item')}>
                        <i className="fa fa-fw fa-list-ul"/>
                    </Button>
                </OverlayTrigger>
                <OverlayTrigger placement="top" overlay={tooltipLink}>
                    <Button bsStyle="link" onClick={() => this.props.onUtilityClick('link')}>
                        <i className="fa fa-fw fa-link"/>
                    </Button>
                </OverlayTrigger>

                <OverlayTrigger placement="top" overlay={tooltipUndo}>
                    <Button className="pull-right" bsStyle="link" onClick={() => this.props.onUtilityClick('redo')}>
                        <i className="fa fa-fw fa-rotate-right"/>
                    </Button>
                </OverlayTrigger>
                <OverlayTrigger placement="top" overlay={tooltipRedo}>
                    <Button className="pull-right" bsStyle="link" onClick={() => this.props.onUtilityClick('undo')}>
                        <i className="fa fa-fw fa-rotate-left"/>
                    </Button>
                </OverlayTrigger>
            </ButtonGroup>
        );
    }
}

RichEditor.propTypes = {
    onUtilityClick: React.PropTypes.func,
    onToggleBlockType: React.PropTypes.func,
    onToggleInlineStyle: React.PropTypes.func
};

export default RichEditor