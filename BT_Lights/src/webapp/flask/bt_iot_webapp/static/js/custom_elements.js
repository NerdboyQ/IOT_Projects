const template = document.createElement('template');
template.innerHTML = `
    <style>
        /* @import url();  for importing custom css*/
        div {
            border-radius: 5px;
            corner-radius: 15px;
        }
        :host {
            /* for the shadow root */
            background-color: #57A0D3;
            border-radius: 5px;
            display: block;
            margin: 20px;
            padding: 20px;
        }
        /* conditional formatting based on the element type */
        :host() {

        }
        /* conditional style based on parent element*/
        :host-context() {

        }
        /* anything that's slotted into the element */
        /* NOTE: This is loaded BEFORE the main css is loaded, so
           so the changes applied to slotted elements will be overwritten.
           to circumvent this, we use the '!important' key to ensure the
           styling is maintained for this custom element.*/
        ::slotted(*){
            color: #7EF9FF !important;
        }
    </style>
    <div>
        <slot name="bt-dev-name">No Title Slot Provided</slot>
        <slot name="bt-addr">No Title Slot Provided</slot>
        <!-- Gives the implementer the ability to override default styles -->
        <h5 part="rando">blah blah</h5>
    </div>
`;
class BluetoothDevContainer extends HTMLElement {
    
    constructor(){
        super();
        console.log("creating bt container")
        // This is the "container/sandbox/canvas" for the custom element
        const shadowRoot = this.attachShadow({mode: 'closed'});

        // let div = document.createElement('div');
        // div.textContent = 'BT DEV Container';
        // shadowRoot.append(div);

        let clone = template.content.cloneNode(true);
        shadowRoot.appendChild(clone);
    }

}

customElements.define('bt-dev-container', BluetoothDevContainer);