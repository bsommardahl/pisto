open Js.Promise;

open OrderHelper;

type viewing =
  | Tags
  | Products(string);

type modifying =
  | Nothing
  | CustomerName;

type state = {
  allProducts: list(Product.t),
  id: option(string),
  customerName: string,
  orderItems: list(OrderItem.t),
  discounts: list(Discount.t),
  createdOn: float,
  lastUpdated: option(float),
  returned: option(Return.t),
  paid: option(Paid.t),
  tags: list(string),
  viewing,
  meta: option(Js.Json.t),
  removed: bool,
  closedOrder: bool,
  modifying,
  allDiscounts: list(Discount.t),
  sku: string,
  showDialog: bool,
  skuEnabled: bool,
};

type action =
  | SelectTag(string)
  | SelectProduct(Product.t)
  | DeselectTag
  | LoadOrder(Order.orderVm)
  | CloseOrderScreen
  | ChangePaidDate(float)
  | ChangeCustomerName(string)
  | ChangeOrderItems(list(OrderItem.t))
  | ProductsLoaded(list(Product.t))
  | DiscountsLoaded(list(Discount.t))
  | ApplyDiscount(Discount.t)
  | RemoveDiscount(Discount.t)
  | ShowDialog
  | HideDialog
  | EnableSku
  | DisableSku;

let dbUrl = "http://localhost:5984/orders";

let component = ReasonReact.reducerComponent("Order");

let buildOrder = state : Order.orderVm => {
  let order: Order.orderVm = {
    id: state.id,
    customerName: state.customerName,
    orderItems: state.orderItems,
    createdOn: state.createdOn,
    discounts: state.discounts,
    paid: state.paid,
    returned: state.returned,
    lastUpdated: state.lastUpdated,
    removed: state.removed,
    meta: state.meta,
  };
  /* order |> Order.fromVm |> WebhookEngine.fireForOrder(OrderStarted) |> ignore; */
  order;
};

let make = (~goBack, _children) => {
  ...component,
  reducer: (action, state) =>
    switch (action) {
    | ProductsLoaded(products) =>
      let tags = Product.getTags(products);
      ReasonReact.Update({...state, tags, allProducts: products});
    | DiscountsLoaded(discounts) =>
      ReasonReact.Update({...state, allDiscounts: discounts})
    | ChangeOrderItems(orderItems) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, orderItems /*List.concat([state.orderItems])*/},
        (_ => Js.log(state.orderItems)),
      )
    | LoadOrder(order) =>
      ReasonReact.Update({
        ...state,
        orderItems: order.orderItems,
        discounts: order.discounts,
        customerName: order.customerName,
        paid: order.paid,
        returned: order.returned,
        id: order.id,
        removed: order.removed,
        lastUpdated: order.lastUpdated,
        createdOn: order.createdOn,
        meta: order.meta,
        closedOrder:
          switch (state.paid) {
          | Some(_) => true
          | None => false
          },
      })
    | ApplyDiscount(dis) =>
      ReasonReact.Update({
        ...state,
        discounts: List.concat([state.discounts, [dis]]),
        allDiscounts:
          state.allDiscounts
          |> List.filter((d: Discount.t) => d.id !== dis.id),
      })
    | RemoveDiscount(dis) =>
      ReasonReact.Update({
        ...state,
        discounts:
          state.discounts |> List.filter((d: Discount.t) => d.id !== dis.id),
        allDiscounts: List.concat([state.allDiscounts, [dis]]),
      })
    | ChangeCustomerName(name) =>
      ReasonReact.Update({...state, customerName: name})
    | ChangePaidDate(date) =>
      ReasonReact.Update({
        ...state,
        paid:
          switch (state.paid) {
          | None => None
          | Some(paid) => Some({...paid, on: date})
          },
      })
    | SelectTag(tag) =>
      ReasonReact.Update({...state, viewing: Products(tag)})
    | DeselectTag => ReasonReact.Update({...state, viewing: Tags})
    | CloseOrderScreen => ReasonReact.SideEffects((_self => goBack()))
    | SelectProduct(product) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          orderItems:
            List.concat([state.orderItems, [buildOrderItem(product)]]),
        },
        (self => self.send(HideDialog)),
      )
    | ShowDialog =>
      ReasonReact.UpdateWithSideEffects(
        {...state, showDialog: true},
        (self => self.send(DisableSku)),
      )
    | HideDialog =>
      ReasonReact.UpdateWithSideEffects(
        {...state, showDialog: false},
        (self => self.send(EnableSku)),
      )
    | EnableSku => ReasonReact.Update({...state, skuEnabled: true})
    | DisableSku => ReasonReact.Update({...state, skuEnabled: false})
    },
  initialState: () => {
    let queryString = ReasonReact.Router.dangerouslyGetInitialUrl().search;
    let customerName =
      switch (Util.QueryParam.get("customerName", queryString)) {
      | Some(name) => name |> Js.Global.decodeURIComponent
      | None => "order.defaultCustomerName" |> Lang.translate
      };
    {
      orderItems: [],
      discounts: [],
      meta: None,
      removed: false,
      createdOn: 0.0,
      lastUpdated: None,
      id: None,
      paid: None,
      customerName,
      returned: None,
      closedOrder: false,
      allProducts: [],
      tags: [],
      viewing: Tags,
      modifying: Nothing,
      allDiscounts: [],
      sku: "",
      showDialog: false,
      skuEnabled: true,
    };
  },
  didMount: self => {
    DiscountStore.getAll()
    |> then_(discounts => {
         self.send(DiscountsLoaded(discounts));
         resolve();
       })
    |> ignore;
    ProductStore.getAll()
    |> then_(prods => {
         self.send(ProductsLoaded(prods));
         resolve();
       })
    |> ignore;
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
    Js.log("orderscreen:: " ++ self.state.customerName);
    let deselectTag = _event => self.send(DeselectTag);
    let selectTag = tag => self.send(SelectTag(tag));
    let selectProduct = product => self.send(SelectProduct(product));
    let discountSelected = discount => self.send(ApplyDiscount(discount));
    let discountDeselected = discount => self.send(RemoveDiscount(discount));
    <div className="order">
      <SearchModal
        label="modal.SearchProductHeader"
        isOpen=self.state.showDialog
        allProducts=self.state.allProducts
        onSelect=(p => self.send(SelectProduct(p)))
        onCancel=(_ => self.send(HideDialog))
      />
      <div className="order-header">
        <OrderActions
          order=(buildOrder(self.state))
          onFinish=(_ => self.send(CloseOrderScreen))
        />
        <div className="order-actions">
          <Button
            local=true
            className="pay-button-card"
            label="order.searchProduct"
            onClick=(_ => self.send(ShowDialog))
          />
        </div>
        <div className="customer-name">
          <EditableText
            mode=TouchToEdit
            text=self.state.customerName
            onChange=(newName => self.send(ChangeCustomerName(newName)))
          />
        </div>
      </div>
      <div className="right-side">
        <SkuSearch
          acceptInput=self.state.skuEnabled
          allProducts=self.state.allProducts
          productFound=(p => self.send(SelectProduct(p)))
        />
        <OrderItems
          closed=self.state.closedOrder
          orderItems=self.state.orderItems
          discounts=self.state.discounts
          deselectDiscount=discountDeselected
          onChange=(orderItems => self.send(ChangeOrderItems(orderItems)))
        />
      </div>
      <div className="left-side">
        (
          if (self.state.closedOrder) {
            <ClosedOrderInfo
              order=(buildOrder(self.state))
              paidDateChanged=(newDate => self.send(ChangePaidDate(newDate)))
            />;
          } else {
            <div>
              (
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
                    <Button
                      local=true
                      className="back-button-card"
                      onClick=deselectTag
                      label="nav.back"
                    />
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
              <div className="discounts">
                <DiscountSelector
                  discounts=self.state.allDiscounts
                  selected=discountSelected
                />
              </div>
              <div className="more-actions">
                <Button
                  local=true
                  onClick=(
                    _ =>
                      WebhookEngine.getWebhooks(PrintOrder, Order)
                      |> WebhookEngine.fire(
                           buildOrder(self.state)
                           |> Order.fromVm
                           |> Order.toJs,
                         )
                      |> ignore
                  )
                  label="order.printOrder"
                />
              </div>
            </div>;
          }
        )
      </div>
    </div>;
  },
};