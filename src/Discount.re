type t = {
  id: string,
  name: string,
  percent: int,
};

module NewDiscount = {
  type t = {
    name: string,
    percent: int,
  };
  let mapToJs = (discount: t) => {
    "name": discount.name,
    "percent": discount.percent,
  };
};

let mapFromJs = discountJs : t => {
  id: discountJs##_id,
  name: discountJs##name,
  percent: discountJs##percent,
};

let mapToJsWithRev = (id: string, rev: option(string), discount: t) => {
  "_id": id,
  "_rev": Js.Nullable.return(rev),
  "name": discount.name,
  "percent": discount.percent,
};