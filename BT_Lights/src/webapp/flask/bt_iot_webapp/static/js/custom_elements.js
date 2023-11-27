class BluetoothDevBox extends HTMLElement {
    constructor(){
        super();
    }

    connectedCallback() {
        // DOM - Document Object Model
        // access the shadow DOM
        const shadow = this.shadowRoot;

        // access an element inside the shadow DOM
        const contentDiv = shadow.querySelector('.custom-element-content');

        // ad an event listener or perform other interactions
        contentDiv.addEventListener('click', () => {
            console.log('Custom element clicked!');
        });
    }

}

customElements.define('bluetoothdevbox', BluetoothDevBox);