open Util;

open OrderData;

open OrderConversion;

type viewing =
  | Tags
  | Products(string);

type modifying =
  | Nothing
  | CustomerName;

type state = {
  allProducts: list(Product.t),
  tags: list(string),
  viewing,
  order: Order.orderVm,
  closedOrder: bool,
  modifying,
};

type action =
  | SelectTag(string)
  | SelectProduct(Product.t)
  | DeselectTag
  | LoadOrder(Order.orderVm)
  | CloseOrderScreen
  | RemoveOrderItem(Order.orderItem)
  | EnableModCustomerName
  | DisableModCustomerName
  | ChangeCustomerName(string);

let buildOrderItem = (product: Product.t) : Order.orderItem => {
  productCode: product.code,
  name: product.name,
  suggestedPrice: product.suggestedPrice,
  addedOn: Js.Date.now(),
  salePrice: product.suggestedPrice,
  taxCalculation: product.taxCalculation,
};

let buildNewOrder = (customerName: string) : Order.orderVm => {
  id: None,
  customerName,
  orderItems: [],
  createdOn: Js.Date.now(),
  paidOn: None,
  amountPaid: None,
  paymentTakenBy: None,
  lastUpdated: None,
  removed: false,
};

let getOrderVm = orderId =>
  CafeStore.get(orderId)
  |> Js.Promise.then_(order => {
       let vm = vmFromExistingOrder(order);
       Js.Promise.resolve(vm);
     });

let dbUrl = "http://localhost:5984/orders";

let component = ReasonReact.reducerComponent("Order");

let make = (~goBack, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | LoadOrder(order) =>
      ReasonReact.Update({
        ...state,
        order,
        closedOrder:
          switch (order.paidOn) {
          | Some(d) => true
          | None => false
          },
      })
    | EnableModCustomerName =>
      ReasonReact.Update({...state, modifying: CustomerName})
    | DisableModCustomerName =>
      ReasonReact.Update({...state, modifying: Nothing})
    | ChangeCustomerName(name) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          customerName: name,
        },
      })
    | SelectTag(tag) =>
      ReasonReact.Update({...state, viewing: Products(tag)})
    | DeselectTag => ReasonReact.Update({...state, viewing: Tags})
    | CloseOrderScreen => ReasonReact.SideEffects((_self => goBack()))
    | RemoveOrderItem(orderItem) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          orderItems:
            state.order.orderItems |> List.filter(i => i !== orderItem),
        },
      })
    | SelectProduct(product) =>
      ReasonReact.Update({
        ...state,
        order: {
          ...state.order,
          orderItems:
            List.concat([
              state.order.orderItems,
              [buildOrderItem(product)],
            ]),
        },
      })
    },
  initialState: () => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let customerName =
      switch (Util.QueryParam.get("customerName", queryString)) {
      | Some(name) => name
      | None => "Cliente"
      };
    let products = Product.getProducts();
    {
      closedOrder: false,
      allProducts: products,
      tags: Product.getTags(products),
      viewing: Tags,
      order: buildNewOrder(customerName),
      modifying: Nothing,
    };
  },
  didMount: self => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    switch (Util.QueryParam.get("orderId", queryString)) {
    | None => ReasonReact.NoUpdate
    | Some(orderId) =>
      getOrderVm(orderId)
      |> Js.Promise.then_(vm => {
           self.send(LoadOrder(vm));
           Js.Promise.resolve();
         })
      |> ignore;
      ReasonReact.NoUpdate;
    };
  },
  render: self => {
    let deselectTag = _event => self.send(DeselectTag);
    let selectTag = tag => self.send(SelectTag(tag));
    let selectProduct = product => self.send(SelectProduct(product));
    let enableModificationCustomerName = () =>
      self.send(EnableModCustomerName);
    let onCustomerNameChange = ev => {
      let value = ReactDOMRe.domElementToObj(ReactEventRe.Form.target(ev))##value;
      self.send(ChangeCustomerName(value));
    };
    <div className="order">
      <div className="order-header">
        <OrderActions
          closed=self.state.closedOrder
          order=self.state.order
          onFinish=((_) => self.send(CloseOrderScreen))
        />
        (
          switch (self.state.modifying) {
          | CustomerName =>
            <div className="customer-name">
              <input
                value=self.state.order.customerName
                onChange=onCustomerNameChange
              />
              <button onClick=((_) => self.send(DisableModCustomerName))>
                (s("Cambiar"))
              </button>
            </div>
          | _ =>
            <div
              className="customer-name"
              onClick=((_) => enableModificationCustomerName())>
              (s(self.state.order.customerName))
            </div>
          }
        )
      </div>
      <div className="left-side">
        (
          if (self.state.closedOrder) {
            <div> (s("Pagado")) </div>;
          } else {
            switch (self.state.viewing) {
            | Tags =>
              <div className="tags">
                (
                  self.state.tags
                  |> List.map(tag => <TagCard onSelect=selectTag tag />)
                  |> Array.of_list
                  |> ReasonReact.arrayToElement
                )
              </div>
            | Products(tag) =>
              <div className="products">
                <div className="back-button-card card" onClick=deselectTag>
                  (s("Atras"))
                </div>
                (
                  Product.filterProducts(tag, self.state.allProducts)
                  |> List.map(product =>
                       <ProductCard onSelect=selectProduct product />
                     )
                  |> Array.of_list
                  |> ReasonReact.arrayToElement
                )
              </div>
            };
          }
        )
      </div>
      <div className="right-side">
        <OrderItems
          closed=self.state.closedOrder
          order=self.state.order
          onRemoveItem=(i => self.send(RemoveOrderItem(i)))
        />
      </div>
    </div>;
  },
};