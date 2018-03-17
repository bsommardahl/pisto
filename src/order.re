open Util;

type viewing =
  | Tags
  | Products(string);

type state = {
  allProducts: list(Product.t),
  tags: list(string),
  viewing,
  order: OrderData.Order.order,
};

type action =
  | Save
  | Pay
  | Delete
  | Close
  | SelectTag(string)
  | SelectProduct(Product.t)
  | DeselectTag;

let buildOrderItem = (product: Product.t) : OrderData.Order.orderItem => {
  product,
  addedOn: Js.Date.now(),
  salePrice: product.suggestedPrice,
};

let buildNewOrder = (customerName: string) : OrderData.Order.order => {
  id: None,
  customerName,
  orderItems: [],
  createdOn: Js.Date.now(),
  paidOn: None,
  amountPaid: None,
  paymentTakenBy: None,
};

let loadExistingOrder = (orderId: int) : OrderData.Order.order => {
  /* get order from store */
  id: Some(string_of_int(orderId)),
  customerName: "n/a",
  orderItems: [],
  createdOn: Js.Date.now(),
  paidOn: None,
  amountPaid: None,
  paymentTakenBy: None,
};

let component = ReasonReact.reducerComponent("Order");

let make = (~finishedWithOrder: OrderData.Order.order => unit, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | SelectTag(tag) =>
      ReasonReact.Update({...state, viewing: Products(tag)})
    | DeselectTag => ReasonReact.Update({...state, viewing: Tags})
    | Save => ReasonReact.NoUpdate
    | Pay => ReasonReact.NoUpdate
    | Delete => ReasonReact.NoUpdate
    | Close => ReasonReact.NoUpdate
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
    let orderId =
      switch (Util.QueryParam.get("orderId", queryString)) {
      | Some(id) => int_of_string(id)
      | None => 0
      };
    let products = Product.getProducts();
    {
      allProducts: products,
      tags: Product.getTags(products),
      viewing: Tags,
      order:
        switch (orderId) {
        | 0 => buildNewOrder(customerName)
        | _ => loadExistingOrder(orderId)
        },
    };
  },
  render: self => {
    let deselectTag = _event => self.send(DeselectTag);
    let selectTag = tag => self.send(SelectTag(tag));
    let selectProduct = product => self.send(SelectProduct(product));
    <div className="Order">
      <h1> (s("Order")) </h1>
      <h2> (s("Name:")) (s(self.state.order.customerName)) </h2>
      (
        switch (self.state.order.id) {
        | None => <div />
        | Some(id) => <h2> (s("Id:")) (s(id)) </h2>
        }
      )
      (
        switch (self.state.viewing) {
        | Tags =>
          self.state.tags
          |> List.map(tag => <div> <TagCard onSelect=selectTag tag /> </div>)
          |> Array.of_list
          |> ReasonReact.arrayToElement
        | Products(tag) =>
          <div>
            <button onClick=deselectTag> (s("Atras")) </button>
            (
              Product.filterProducts(tag, self.state.allProducts)
              |> List.map(product =>
                   <ProductCard onSelect=selectProduct product />
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </div>
        }
      )
      <OrderItems orderItems=self.state.order.orderItems />
      <OrderTotals orderItems=self.state.order.orderItems />
      <OrderActions order=self.state.order onFinish=finishedWithOrder />
    </div>;
  },
};